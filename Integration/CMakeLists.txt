cmake_minimum_required(VERSION 3.27) #Checks that the user has cmake version 3.27 or newer

project(smk_ai_emulation_core CXX) #Project name and specifying cpp

set(CMAKE_CXX_STANDARD 19) #Compiler will use cpp19


set(SNES9X_SOURCES
    cpu.cpp         #Emulate CPU
    ppu.cpp         #Emulate Graphics
    apu.cpp         #Emulate Audio
    memmap.cpp      #Memory and Address
    controls.cpp    #Player(AI) Inputs
    gfx.cpp         #Graphics Output
    dsp.cpp         #Needed for Super MarioKart
    dsp1.cpp        #Also Needed
)


add_library(smk_ai_emulation_core STATIC ${SNES9X_SOURCES}) #Compiles sources into a static library


#target_include_directories tells the compiler where to look 
#for header files associated with this (may need changed)
target_include_directories(smk_ai_emulation_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
