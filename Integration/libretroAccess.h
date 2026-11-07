#pragma once

//Gemini -
#include <string>
#include <SDL.h>
#include <cstdint>
//- Gemini

#define RETRO_API //Gemini

#include "../libretro/libretro.h"

//Gemini -

class libretroAccess {
private:
    enum retro_pixel_format core_pixel_format = RETRO_PIXEL_FORMAT_0RGB1555;

    unsigned int* current_buttons_ptr = nullptr;
    static libretroAccess* instance;

    static bool env_cb(unsigned cmd, void* data);
    static void video_cb(const void* data, unsigned w, unsigned h, size_t pitch);
    static void audio_cb(int16_t l, int16_t r);
    static size_t audio_batch_cb(const int16_t* data, size_t frames);
    static void input_poll_cb(void);
    static int16_t input_state_cb(unsigned port, unsigned device, unsigned index, unsigned id);

    // SDL Window Components ---
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

public:
    libretroAccess();
    ~libretroAccess();

    bool Initialize(unsigned int& button_ref);
    bool InitVideo(int width = 512, int height = 448); // Sets up the SDL window
    bool LoadROM(const std::string& path);
    void Reset();
    void RunFrame();

    uint8_t* getRAM();
    size_t getRAMsize();
}; //- Gemini