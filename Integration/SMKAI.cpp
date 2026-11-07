
//Gemini -
#define SDL_MAIN_HANDLED
#include <SDL.h>
//- Gemini

#include <iostream>
#include <string>

//Gemini -
#include <limits>
#include <chrono>
#include <thread>
//- Gemini

#include "libretroAccess.h"
#include "mutate.h"
#include "buttons.h"
#include "aiSegments/AIgorithm.h"


int main(int argc, char* argv[]) {
    std::cout << "Program Started!" << std::endl;

    bool manyAI = argc > 1;

    int AID = 0;
    if (manyAI) {
        AID = std::stoi(argv[1]); //Gemini
    }
    else {
        double tempRate;
        std::cout << std::endl << "Please enter in decimal form the mutation rate: ";
        std::cin >> tempRate;
        mutate::setMutate(tempRate);

        //Gemini -
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        //- Gemini
    }

    //Gemini -
    // 1. Initialize Emulator Wrapper
    libretroAccess snes;
    unsigned int current_buttons = 0;

    if (!snes.Initialize(current_buttons)) {
        std::cout << "Failed to initialize Libretro!\n";
        std::cout << "Ensure libretro.ccp/h are placed correctly in your executable's working directory.\n";
        return -1;
    }
    std::cout << "Libretro Core Initialized Successfully!\n";
    //- Gemini


    std::string rompath;
    int watchSpeed = 1;
    bool showGameplay = false;
    if (!manyAI) {
        std::cout << std::endl << "Enter the file path of the ROM here (.sfc or .smc):" << std::endl;
        getline(std::cin, rompath);
        if (!rompath.empty() && rompath.front() == '\"' && rompath.back() == '\"') {
            rompath = rompath.substr(1, rompath.size() - 2);
        }


        std::string showAI;
        std::cout << std::endl << "Would you like to watch the AI race? (y) (case sensitive):" << std::endl;
        std::cin >> showAI;
        showGameplay = (showAI == "y"); // (Can be used later if you hook up video rendering)

        if (showGameplay) {
            std::cout << std::endl << "Speed Options For Viewing:" << std::endl <<
                "Half Speed (0)" << std::endl <<
                "Normal Speed (1)" << std::endl <<
                "Double Speed (2)" << std::endl <<
                "Triple Speed (3)" << std::endl <<
                "Quadruple Speed (4)" << std::endl <<
                "Please enter your selection (int): ";
            std::cin >> watchSpeed;
        }

        //Gemini -
        // NEW: Initialize video window if requested ---
        if (showGameplay) {
            // Standard SNES resolution is 256x224 (or 256x239)
            if (!snes.InitVideo(256, 224)) {
                std::cout << "Failed to initialize video window. Falling back to headless mode.\n";
                showGameplay = false;
            }
        } //- Gemini
    }


    //Gemini -
    // 2. Load ROM
    if (!snes.LoadROM(rompath)) {
        std::cout << "Failed to load ROM: " << rompath << "\n";
        return -1;
    }
    std::cout << "ROM Loaded Successfully via Libretro!\n";

    // Tell Snes9x that Port 0 has a standard SNES Joypad connected
    retro_set_controller_port_device(0, RETRO_DEVICE_JOYPAD);

    // 3. Reset Core
    snes.Reset();
    std::cout << "SNES Core Reset Successfully!\n";
    //- Gemini

    
    AIgorithm theAIgorithm;
    if (manyAI) {
        theAIgorithm = AIgorithm(AID);
        std::cout << "AI " << AID << " of " << argc << " up and ready!" << std::endl;
    }
    else {
        theAIgorithm = AIgorithm();
        std::cout << std::endl << "AI up and ready!" << std::endl;
    }


    std::cout << std::endl << "Begin Emulation" << std::endl;



    // --- Main AI Execution Loop ---
    bool run = true;
    int frameCount = 0;
    int buttonSelection = 0;

    /*
    FYI lower number for targetFPS is faster running
    As seen below Gemini says 60FPSish is 16639ish
    This means half speed is about 33278
    And double speed is about 8320
    Meaning quadruple is 4160
    */
    int targetFPS = 0;
    switch (watchSpeed) {

    case 0:
        targetFPS = 33278;
        break;

    case 1:
        targetFPS = 16639;
        break;

    case 2:
        targetFPS = 8320;
        break;

    case 3:
        targetFPS = 5547;
        break;

    case 4:
        targetFPS = 4160;
        break;

    case 5:
        targetFPS = 3328;
        break;

    default:
        targetFPS = 16639;
        break;
    }

    //Gemini -
    // Target ~60.09 FPS for SNES (approx. 16,639 microseconds per frame)
    const auto frameDuration = std::chrono::microseconds(targetFPS);
    auto nextFrameTime = std::chrono::steady_clock::now() + frameDuration;
    //- Gemini

    while (run) {
        //Gemini -
        // --- NEW: Handle SDL window events so the window doesn't freeze ---
        if (showGameplay) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    run = false; // Gracefully quit if user clicks the X button
                }
            }
        } //- Gemini

        if (frameCount < 800) { //Startup Race
            if (frameCount % 30 == 0) {
                buttonSelection = 0;
            }
            else {
                buttonSelection = 21;
            }
        }
        else if (mutate::mutation()) {
            buttonSelection = mutate::pressThis();
        }
        else { //AI Action
            buttonSelection = theAIgorithm.aiButtonChoice();
        }

        //Gemini -
        // Update the button state that libretroAccess will query on the next frame
        current_buttons = buttons::intToButton(buttonSelection);

        // Advance emulator by exactly one frame (single-threaded & deterministic)
        snes.RunFrame();
        //- Gemini

        frameCount++;

        // Periodic frame and score tracking check
        if (frameCount % 300 == 0) {
            std::cout << "Frames executed: " << frameCount << " \t ";
            theAIgorithm.aiFeedback();
        }

        //Gemini -
        // Only pace to 60 FPS if the user is watching the visual window
        if (showGameplay) {
            std::this_thread::sleep_until(nextFrameTime);
            nextFrameTime += frameDuration;
        } //- Gemini
    }

    return 0;
}