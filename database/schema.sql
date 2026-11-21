CREATE TABLE IF NOT EXISTS runs (
    run_id INTEGER PRIMARY KEY AUTOINCREMENT,
    started_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    algorithm TEXT NOT NULL,
    learning_rate REAL NOT NULL,
    gamma REAL NOT NULL,
    -- Snapshot of your scoring.cpp weights so you can compare reward tuning
    reward_weights_json TEXT NOT NULL,
    notes TEXT
);

CREATE TABLE IF NOT EXISTS episodes (
    episode_id INTEGER PRIMARY KEY AUTOINCREMENT,
    run_id INTEGER NOT NULL,
    episode_num INTEGER NOT NULL,
    track_name TEXT,
    
    -- Final scalar from scoring::getScore()
    final_score REAL NOT NULL,

    -- Some methods referenced below are commented out in their original file, but we will still add them for when we eventually need them.
    -- In particular, the trackers for coins collection and item collection/usage are not yet being used. 
  
    -- Positive progress events
    checkpoints_forward INTEGER DEFAULT 0,   -- checkPointCrossing(many >= 0) -> +10
    laps_completed INTEGER DEFAULT 0,        -- completedLap(place) -> +(10 - place) * 3
    best_lap_place INTEGER,                  -- Lowest (best) place recorded on lap completion
    coins_collected INTEGER DEFAULT 0,       -- collectedCoin()
    items_picked INTEGER DEFAULT 0,          -- pickedItem()
    items_used INTEGER DEFAULT 0,            -- usedItem()
    
    -- Penalty / mistake events
    checkpoints_backward INTEGER DEFAULT 0,  -- checkPointCrossing(many < 0) -> -11
    coins_lost INTEGER DEFAULT 0,            -- lostCoin() -> -1
    item_hits INTEGER DEFAULT 0,             -- hitByItem() -> -2
    fell_off_map INTEGER DEFAULT 0,          -- fellOffMap() -> -5
    spin_outs INTEGER DEFAULT 0,             -- spunOut() -> -1
    collisions INTEGER DEFAULT 0,            -- collision() -> -1
    
    -- Episode metadata
    total_steps INTEGER DEFAULT 0,
    epsilon REAL,
    FOREIGN KEY (run_id) REFERENCES runs(run_id)
);

CREATE INDEX IF NOT EXISTS idx_episodes_run ON episodes(run_id, episode_num);
