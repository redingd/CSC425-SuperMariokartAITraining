#include "libretroAccess.h"

//Gemini -
#include <iostream>

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
    if (cmd == RETRO_ENVIRONMENT_GET_CAN_DUPE && data) {
        *(bool*)data = true;
    }
    return true;
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
    struct retro_game_info game = { path.c_str(), nullptr, 0, nullptr };
    return retro_load_game(&game);
}

void libretroAccess::Reset() {
    retro_reset();
}

void libretroAccess::RunFrame() {
    retro_run();
}
//- Gemini