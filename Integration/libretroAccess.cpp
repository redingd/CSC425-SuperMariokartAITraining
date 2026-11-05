#include "libretroAccess.h"

//Gemini -
#include <iostream>
#include <fstream>
#include <vector>

// Declare the Libretro core functions directly (provided by compiling the Snes9x libretro source files)
extern "C" {
    void retro_init(void);
    void retro_deinit(void);
    bool retro_load_game(const struct retro_game_info* game);
    void retro_reset(void);
    void retro_run(void);
    void retro_set_environment(retro_environment_t);
    void retro_set_video_refresh(retro_video_refresh_t);
    void retro_set_audio_sample(retro_audio_sample_t);
    void retro_set_audio_sample_batch(retro_audio_sample_batch_t);
    void retro_set_input_poll(retro_input_poll_t);
    void retro_set_input_state(retro_input_state_t);
}

libretroAccess* libretroAccess::instance = nullptr;

libretroAccess::libretroAccess() {
    instance = this;
}

libretroAccess::~libretroAccess() {
    retro_deinit();
    if (instance == this) instance = nullptr;
}

bool libretroAccess::env_cb(unsigned cmd, void* data) {
    switch (cmd) {
    case RETRO_ENVIRONMENT_GET_CAN_DUPE:
        if (data) *(bool*)data = true;
        return true;

    case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
        // Snes9x tells you what pixel format it wants (e.g., 0x1 for RGB565, 0x0 for XRGB8888)
        // You can accept it by returning true.
        return true;

    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
    case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
    case RETRO_ENVIRONMENT_GET_CORE_ASSETS_DIRECTORY: {
        // Cores often query this to know where to save SRAM or config files.
        // Returning false or providing a dummy current-directory path prevents crashes.
        if (data) {
            // Point it to a safe string like current directory "."
            *(const char**)data = ".";
        }
        return true;
    }

    case RETRO_ENVIRONMENT_GET_VARIABLE:
        // If the core asks for core options/variables and you don't supply them, 
        // returning false lets the core fall back to its defaults gracefully.
        return false;

    default:
        // Log unhandled commands during debugging if needed, but return false or true safely
        return false;
    }
}

void libretroAccess::video_cb(const void*, unsigned, unsigned, size_t) {}
void libretroAccess::audio_cb(int16_t, int16_t) {}
size_t libretroAccess::audio_batch_cb(const int16_t*, size_t frames) { return frames; }
void libretroAccess::input_poll_cb() {}

int16_t libretroAccess::input_state_cb(unsigned port, unsigned device, unsigned, unsigned id) {
    if (port != 0 || device != RETRO_DEVICE_JOYPAD || !instance || !instance->current_buttons_ptr)
        return 0;

    bool pressed = (*(instance->current_buttons_ptr) & (1 << id)) != 0;
    return pressed ? 1 : 0;
}

bool libretroAccess::Initialize(unsigned int& button_ref) {
    current_buttons_ptr = &button_ref;

    // Register callbacks directly
    retro_set_environment(env_cb);
    retro_set_video_refresh(video_cb);
    retro_set_audio_sample(audio_cb);
    retro_set_audio_sample_batch(audio_batch_cb);
    retro_set_input_poll(input_poll_cb);
    retro_set_input_state(input_state_cb);

    // Initialize the core directly
    retro_init();
    return true;
}

bool libretroAccess::LoadROM(const std::string& path) {
    // 1. Open the ROM file in binary mode and seek to the end to get its size
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cout << "Error: Could not open ROM file on disk: " << path << "\n";
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // 2. Read the file contents into a heap-allocated vector 
    // (Note: Keep this vector alive or managed so the core can read it!)
    // If your class owns this vector, store it as a member variable in libretroAccess.h!
    static std::vector<char> rom_data; // Or store as private member: std::vector<char> m_romData;
    rom_data.resize(size);

    if (!file.read(rom_data.data(), size)) {
        std::cout << "Error: Failed to read ROM file into memory.\n";
        return false;
    }

    // 3. Populate retro_game_info with BOTH path and the loaded memory buffer
    struct retro_game_info game = {};
    game.path = path.c_str();
    game.data = rom_data.data();
    game.size = size;
    game.meta = nullptr;

    // 4. Call the core's load function
    return retro_load_game(&game);
}

void libretroAccess::Reset() {
    retro_reset();
}

void libretroAccess::RunFrame() {
    retro_run();
}
//- Gemini