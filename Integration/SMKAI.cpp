// SMKAI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Gemini
#define _HAS_STD_BYTE 0
#include <windows.h>
//Gemini

#include "../snes9x.h"
#include "../memmap.h"
#include "scoring.h"
#include "mutate.h"

#include <iostream>
#include <string>

#include <cstdlib> //Gemini aided line
#include <limits> //Gemini

using namespace std;

// Provide the mandatory stubs Snes9x demands
bool S9xOpenSoundDevice(void) {
    return false; // Turn sound off for AI, Snes9x checks this
} //Gemini
extern "C" HRESULT WINAPI DXTraceW(const char* strFile, DWORD dwLine, HRESULT hr, const WCHAR* strMsg, BOOL bPopMsgBox) {
    return hr;
}//Gemini

int main() {

    cout << "Program Started!" << endl;

    double tempRate;
    cout << endl << "Please enter in decimal form the mutation rate: ";
    cin >> tempRate;
    mutate::setMutate(tempRate);
    
    
    // 1. Initialize Snes9x memory
    if (Memory.Init()) {
        cout << "SNES Memory Initialized Successfully!\n";
    }
    else {
        cout << "Failed to initialize memory.\n";
        return -1;
    } //Gemini

    cin.clear(); //Gemini
    cin.ignore((numeric_limits<streamsize>::max)(), '\n'); //Gemini

    string rompath;
    cout << endl << "Enter the file path of the rom here:" << endl;
    getline(cin, rompath);
      
    // 1.5. Load the ROM file
    if (!Memory.LoadROM(rompath.c_str())) {
        cout << "Failed to load ROM: " << rompath << "\n";
        return -1;
    } //Gemini
    cout << "ROM Loaded Successfully!\n";
    
    // 2. Reset the emulator core
    S9xReset();
    cout << "SNES Core Reset Successfully!\n";
    //Gemini

    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
