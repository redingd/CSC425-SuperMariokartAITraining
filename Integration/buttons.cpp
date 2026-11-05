#include "buttons.h"
#include "../libretro/libretro.h" // Include libretro headers for standard joypad IDs

using namespace std;

namespace buttons {

    unsigned int intToButton(int toPress) {
        unsigned int pressed = 0;
        switch (toPress) {

        case 0: // Press B, Move Forward
            return (1 << RETRO_DEVICE_ID_JOYPAD_B); //Gemini

        case 1: // Press B and Left dPad
            return (1 << RETRO_DEVICE_ID_JOYPAD_B) | (1 << RETRO_DEVICE_ID_JOYPAD_LEFT); //Gemini

        case 2: // Press B and Right dPad
            return (1 << RETRO_DEVICE_ID_JOYPAD_B) | (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT);

        case 3: // Press A, Use Item
            return (1 << RETRO_DEVICE_ID_JOYPAD_A);

        case 4: // Press B and A
            return (1 << RETRO_DEVICE_ID_JOYPAD_B) | (1 << RETRO_DEVICE_ID_JOYPAD_A);

        case 5: // Press X, Flip Rear View
            return (1 << RETRO_DEVICE_ID_JOYPAD_X);

        case 6: // Press B and X
            return (1 << RETRO_DEVICE_ID_JOYPAD_B) | (1 << RETRO_DEVICE_ID_JOYPAD_X);

        case 7: // Press Left Trigger, Hop
            return (1 << RETRO_DEVICE_ID_JOYPAD_L);

        case 8: // Press B and Left Trigger
            return (1 << RETRO_DEVICE_ID_JOYPAD_B) | (1 << RETRO_DEVICE_ID_JOYPAD_L);

        case 9: // Press Right Trigger, Hop
            return (1 << RETRO_DEVICE_ID_JOYPAD_R);

        case 10: // Press B and Right Trigger
            return (1 << RETRO_DEVICE_ID_JOYPAD_B) | (1 << RETRO_DEVICE_ID_JOYPAD_R);

        case 11: // Press B, Left Trigger, Right Trigger, Power Slide
            return (1 << RETRO_DEVICE_ID_JOYPAD_B) | (1 << RETRO_DEVICE_ID_JOYPAD_L) | (1 << RETRO_DEVICE_ID_JOYPAD_R);

        case 12: // Press B, Left Trigger, Right Trigger, Left dPad
            return (1 << RETRO_DEVICE_ID_JOYPAD_B) | (1 << RETRO_DEVICE_ID_JOYPAD_L) | (1 << RETRO_DEVICE_ID_JOYPAD_R) | (1 << RETRO_DEVICE_ID_JOYPAD_LEFT);

        case 13: // Press B, Left Trigger, Right Trigger, Right dPad
            return (1 << RETRO_DEVICE_ID_JOYPAD_B) | (1 << RETRO_DEVICE_ID_JOYPAD_L) | (1 << RETRO_DEVICE_ID_JOYPAD_R) | (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT);

        case 14: // Press Y, Brake/Backwards
            return (1 << RETRO_DEVICE_ID_JOYPAD_Y);

        case 15: // Press Y and Left dPad
            return (1 << RETRO_DEVICE_ID_JOYPAD_Y) | (1 << RETRO_DEVICE_ID_JOYPAD_LEFT);

        case 16: // Press Y and Right dPad
            return (1 << RETRO_DEVICE_ID_JOYPAD_Y) | (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT);

        case 17: // Press Y and A
            return (1 << RETRO_DEVICE_ID_JOYPAD_Y) | (1 << RETRO_DEVICE_ID_JOYPAD_A);

        case 18: // Press Y and X
            return (1 << RETRO_DEVICE_ID_JOYPAD_Y) | (1 << RETRO_DEVICE_ID_JOYPAD_X);

        case 19: // Press Y and Left Trigger
            return (1 << RETRO_DEVICE_ID_JOYPAD_Y) | (1 << RETRO_DEVICE_ID_JOYPAD_L);

        case 20: // Press Y and Right Trigger
            return (1 << RETRO_DEVICE_ID_JOYPAD_Y) | (1 << RETRO_DEVICE_ID_JOYPAD_R);

        case 21: // No press
            return 0;

        default: // Also no press
            return 0;
        }
    }

}