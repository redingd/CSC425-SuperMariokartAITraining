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
    void retro_set_controller_port_device(unsigned port, unsigned device);
    void retro_get_system_av_info(struct retro_system_av_info* info);
    // --- ADDED: Libretro Memory Prototypes ---
    void* retro_get_memory_data(unsigned id);
    size_t retro_get_memory_size(unsigned id);
}

libretroAccess* libretroAccess::instance = nullptr;

libretroAccess::libretroAccess() {
    instance = this;
}

libretroAccess::~libretroAccess() {
    // Clean up SDL resources if they were initialized
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();

    retro_deinit();
    if (instance == this) instance = nullptr;
}

bool libretroAccess::env_cb(unsigned cmd, void* data) {
    switch (cmd) {
    case RETRO_ENVIRONMENT_GET_CAN_DUPE:
        if (data) *(bool*)data = true;
        return true;

    case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT: {
        if (data) {
            enum retro_pixel_format* pix_fmt = (enum retro_pixel_format*)data;
            *pix_fmt = RETRO_PIXEL_FORMAT_XRGB8888; // Force 32-bit output from Snes9x
            instance->core_pixel_format = *pix_fmt;
            return true;
        }
        return false;
    }

    case RETRO_ENVIRONMENT_GET_OVERSCAN:
        if (data) *(bool*)data = false;
        return true;

    case RETRO_ENVIRONMENT_GET_VARIABLE: {
        struct retro_variable* var = (struct retro_variable*)data;
        if (var && var->key) {
            std::string key(var->key);

            if (key.rfind("snes9x_layer_", 0) == 0 ||
                key == "snes9x_gfx_clip" ||
                key == "snes9x_gfx_transp" ||
                key == "snes9x_hires_blend") {
                // Must be a persistent static string pointer
                static const char* enabled_str = "enabled";
                var->value = enabled_str;
                return true;
            }
        }
        return false;
    }

    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
    case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
    case RETRO_ENVIRONMENT_GET_CORE_ASSETS_DIRECTORY: {
        // Cores often query this to know where to save SRAM or config files.
        if (data) {
            // Point it to a safe string like current directory "."
            *(const char**)data = ".";
        }
        return true;
    }

    case RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL:
        // The core reports its performance requirements; we acknowledge it happily.
        return true;

    case RETRO_ENVIRONMENT_GET_AUDIO_VIDEO_ENABLE:
        // Confirm that both audio and video subsystems are active and enabled
        if (data) *(int*)data = (1 << 0) | (1 << 1); // Enable both Audio (bit 0) and Video (bit 1)
        return true;

    default:
        // Log unhandled commands during debugging if needed, but return false safely
        return false;
    }
}

void libretroAccess::video_cb(const void* data, unsigned width, unsigned height, size_t pitch) {
    // If video wasn't initialized (running headlessly), or data is missing, skip rendering
    // FIX: Only check data, instance, and renderer here. Allow texture to be null so it can be created!
    if (!data || !instance || !instance->renderer) {
        std::cout << "[DEBUG] video_cb skipped (null pointer)!\n";
        return;
    }

    // --- ADDED: Capture pixels for AI without touching SDL rendering ---
    // --- ADDED: Downsample pixels for AI (e.g., to a 32x32 grid) ---
    const uint16_t* rawPixels = static_cast<const uint16_t*>(data);

    int targetW = 32;
    int targetH = 32;
    instance->latestPixelState.resize(targetW * targetH);

    for (int y = 0; y < targetH; ++y) {
        int srcY = (y * height) / targetH;
        for (int x = 0; x < targetW; ++x) {
            int srcX = (x * width) / targetW;

            // Get pixel from row pitch (pitch is in bytes, divide by 2 for uint16_t)
            uint16_t pixel = rawPixels[srcY * (pitch / 2) + srcX];

            // Normalize 16-bit color to 0.0f - 1.0f range
            float intensity = static_cast<float>(pixel & 0x7FFF) / 32767.0f;

            instance->latestPixelState[y * targetW + x] = intensity;
        }
    }
    // -----------------------------------------------------------------

    // Diagnostic: Scan the first few rows to see if there's *any* non-zero data
    const uint16_t* pixels = static_cast<const uint16_t*>(data);
    int nonZeroCount = 0;
    uint32_t samplePixel = 0;

    for (int i = 0; i < (int)(width * height); i++) {
        if (pixels[i] != 0) {
            if (nonZeroCount == 0) samplePixel = pixels[i];
            nonZeroCount++;
        }
    }


    // Create texture if needed
    if (!instance->texture) {
        std::cout << "[DEBUG] Creating 16-bit texture with RGB565 format\n";

        instance->texture = SDL_CreateTexture(
            instance->renderer,
            SDL_PIXELFORMAT_RGB565, // <-- Correct format matching Snes9x's 16-bit native output
            SDL_TEXTUREACCESS_STREAMING,
            width,
            height
        );
        if (!instance->texture) {
            std::cout << "[DEBUG] Failed to create texture! SDL Error: " << SDL_GetError() << "\n";
            return;
        }
        std::cout << "[DEBUG] Texture successfully created: " << width << "x" << height << "\n";
    }

    // 1. Update texture using the core's native pitch (restores full 256 width!)
    SDL_UpdateTexture(instance->texture, nullptr, data, (int)pitch);

    // 2. Clear the screen, copy the texture to the renderer, and present it
    SDL_RenderClear(instance->renderer);
    SDL_RenderCopy(instance->renderer, instance->texture, nullptr, nullptr);
    SDL_RenderPresent(instance->renderer);
}

void libretroAccess::audio_cb(int16_t, int16_t) {}

size_t libretroAccess::audio_batch_cb(const int16_t* data, size_t frames) {
    return frames;
}

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
    static std::vector<char> rom_data;
    rom_data.resize(size);

    if (!file.read(rom_data.data(), size)) {
        std::cout << "Error: Failed to read ROM file into memory.\n";
        return false;
    }

    // 3. Populate retro_game_info with path and memory buffer
    struct retro_game_info game = {};
    game.path = path.c_str();
    game.data = rom_data.data();
    game.size = size;
    game.meta = nullptr;

    // 4. Call the core's load function FIRST
    if (!retro_load_game(&game)) {
        std::cout << "Error: retro_load_game failed.\n";
        return false;
    }
    std::cout << "ROM Loaded Successfully via Libretro!\n";

    // 5. Query AV Info AFTER the game is loaded so the core configures its PPU/video clock
    struct retro_system_av_info av_info = {};
    retro_get_system_av_info(&av_info);


    return true;
}

void libretroAccess::Reset() {
    retro_reset();
}

void libretroAccess::RunFrame() {
    retro_run();
}

bool libretroAccess::InitVideo(int width, int height) {
    // Initialize SDL Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // 1. Add SDL_WINDOW_RESIZABLE to the window creation flags
    window = SDL_CreateWindow(
        "SMKAI - AI Playing Super MarioKart",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width * 2,
        height * 2,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE // <-- Enables mouse resizing and maximize buttons
    );
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // Create a hardware-accelerated renderer with VSync enabled
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    // 2. Lock the logical presentation size to the SNES base resolution (256x224).
    // This tells SDL to automatically handle scaling and letterboxing when the user 
    // drags the window borders, keeping Mario and the game world perfectly proportioned.
    SDL_RenderSetLogicalSize(renderer, width, height);

    // Defer texture creation to video_cb
    texture = nullptr;

    std::cout << "SDL Resizable Video Window Initialized Successfully!\n";
    return true;
}

uint8_t* libretroAccess::getRAM() {
    // RETRO_MEMORY_SYSTEM_RAM gives you access to the SNES WRAM (typically 128KB)
    return static_cast<uint8_t*>(retro_get_memory_data(RETRO_MEMORY_SYSTEM_RAM));
}
size_t libretroAccess::getRAMsize() {
    return retro_get_memory_size(RETRO_MEMORY_SYSTEM_RAM);
}

std::vector<float> libretroAccess::getPixels() {
    return latestPixelState;
}
//- Gemini