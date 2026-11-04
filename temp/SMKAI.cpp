
#include <iostream>
#include <string>

#include <limits> //Gemini

#include "libretroAccess.h"
#include "scoring.h"
#include "mutate.h"
#include "buttons.h"


int main() {
    std::cout << "Program Started!" << std::endl;

    double tempRate;
    std::cout << std::endl << "Please enter in decimal form the mutation rate: ";
    std::cin >> tempRate;
    mutate::setMutate(tempRate);

    //Gemini -
    // 1. Initialize Emulator Wrapper
    libretroAccess snes;
    unsigned int current_buttons = 0;

    if (!snes.Initialize(current_buttons)) {
        std::cout << "Failed to initialize Libretro and snes9x_libretro.dll!\n";
        std::cout << "Ensure snes9x_libretro.dll is built and placed in your executable's working directory.\n";
        return -1;
    }
    std::cout << "Libretro Core Initialized Successfully!\n";

    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    //- Gemini

    std::string rompath;
    std::cout << std::endl << "Enter the file path of the ROM here:" << std::endl;
    getline(std::cin, rompath);
    if (!rompath.empty() && rompath.front() == '\"' && rompath.back() == '\"') {
        rompath = rompath.substr(1, rompath.size() - 2);
    }

    //Gemini -
    // 2. Load ROM
    if (!snes.LoadROM(rompath)) {
        std::cout << "Failed to load ROM: " << rompath << "\n";
        return -1;
    }
    std::cout << "ROM Loaded Successfully via Libretro!\n";

    // 3. Reset Core
    snes.Reset();
    std::cout << "SNES Core Reset Successfully!\n";
    //- Gemini

    // User Ready Check
    std::string ready;
    std::cout << std::endl << "Are you ready to run: (y) (case sensitive) (quits otherwise):" << std::endl;
    std::cin >> ready;
    if (ready != "y") {
        std::cout << std::endl << "User Quit" << std::endl;
        return 0;
    }

    std::string showAI;
    std::cout << std::endl << "Would you like to watch the AI race? (y) (case sensitive):" << std::endl;
    std::cin >> showAI;
    bool showGameplay = (showAI == "y"); // (Can be used later if you hook up video rendering)


    std::cout << std::endl << "Begin Emulation" << std::endl;



    // --- Main AI Execution Loop ---
    bool run = true;
    int frameCount = 0;
    int buttonSelection = 0;

    while (run) {
        if (mutate::mutation()) {
            buttonSelection = mutate::pressThis();
        }
        else {
            // Future AI policy output here
            buttonSelection = 0;
        }

        //Gemini -
        // Update the button state that libretroAccess will query on the next frame
        current_buttons = buttons::intToButton(buttonSelection);

        // Advance emulator by exactly one frame (single-threaded & deterministic)
        snes.RunFrame();
        //- Gemini

        frameCount++;

        // Periodic logging and score tracking check
        if (frameCount % 100 == 0) {
            std::cout << "Frames executed: " << frameCount << " | Current Score: " << scoring::score << "\n";
        }
    }

    return 0;
}