// SMKAI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Gemini
#define _HAS_STD_BYTE 0
#include <windows.h>
//Gemini

#include "../snes9x.h"
#include "../memmap.h"
#include "../controls.h"
#include "scoring.h"
#include "mutate.h"
#include "buttons.h"

#include <iostream>
#include <string>

#include <cstdlib> //Gemini aided line
#include <limits> //Gemini


// Provide the mandatory stubs Snes9x demands
bool S9xOpenSoundDevice(void) {
    return false; // Turn sound off for AI, Snes9x checks this
} //Gemini
extern "C" HRESULT WINAPI DXTraceW(const char* strFile, DWORD dwLine, HRESULT hr, const WCHAR* strMsg, BOOL bPopMsgBox) {
    return hr;
}//Gemini

int main() {

    std::cout << "Program Started!" << std::endl;

    double tempRate;
    std::cout << std::endl << "Please enter in decimal form the mutation rate: ";
    std::cin >> tempRate;
    mutate::setMutate(tempRate);
    
    
    // 1. Initialize Snes9x memory
    if (Memory.Init()) {
        std::cout << "SNES Memory Initialized Successfully!\n";
    }
    else {
        std::cout << "Failed to initialize memory.\n";
        return -1;
    } //Gemini

    std::cin.clear(); //Gemini
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); //Gemini

    std::string rompath;
    std::cout << std::endl << "Enter the file path of the rom here:" << std::endl;
    getline(std::cin, rompath);
      
    // 1.5. Load the ROM file
    if (!Memory.LoadROM(rompath.c_str())) {
        std::cout << "Failed to load ROM: " << rompath << "\n";
        return -1;
    } //Gemini
    std::cout << "ROM Loaded Successfully!\n";
    
    // 2. Reset the emulator core
    S9xReset();
    std::cout << "SNES Core Reset Successfully!\n";
    //Gemini


    //Check User is Ready
    std::string ready;
    std::cout << std::endl << "Are you ready to run: (y) (case sensative) (quits otherwise):" << std::endl;
    std::cin >> ready;
    if (ready != "y") {
        std::cout << std::endl << "User Quit" << std::endl;
        return 0;
    }



    std::cout << std::endl << "Begin Emulation" << std::endl;

    //Begin Emulation!!!
    S9xSetController(0, CTL_JOYPAD, 0, 0, 0, 0); //Gemini
    bool run = true;
    int buttonSelection;
    unsigned int pressing;
    while (run) {
        if (mutate::mutation()) {
            buttonSelection = mutate::pressThis();
        }
        else {
            //Will be where AI code is called
            //For now just B to demonstrate usability
            buttonSelection = 0;
        }
        pressing = buttons::intToButton(buttonSelection);
        
        //Gemini -
        // Build the command structure for Player 1
        s9xcommand_t cmd;
        cmd.type = S9xButtonJoypad;
        cmd.multi_press = 0;
        cmd.button_norpt = 0;
        cmd.button.joypad.idx = 0;        // Player 1
        cmd.button.joypad.toggle = 0;
        cmd.button.joypad.turbo = 0;
        cmd.button.joypad.sticky = 0;
        cmd.button.joypad.buttons = pressing; //Partial Gemini, Partial Coder

        // Apply command (data1 = 1 if buttons are pressed, 0 if released/none)
        S9xApplyCommand(cmd, pressing != 0 ? 1 : 0, 0);

        // Run a single frame of the emulator
        S9xMainLoop();
        //- Gemini

        //Will need to implement a way to safely user exist the while loop once starting to implement AI
        
    }



    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
