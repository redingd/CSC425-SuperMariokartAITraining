#include "buttons.h"
#include "../snes9x.h"

using namespace std;

namespace buttons {

	unsigned int intToButton(int toPress) {
		unsigned int pressed = 0;
		switch (toPress) {

		case 0: //Press B, Move Forward
			return SNES_B_MASK; //Gemini

		case 1: //Press B and Left dPad
			return SNES_B_MASK | SNES_LEFT_MASK; //Gemini

		case 2: //Press B and Right dPad
			return SNES_B_MASK | SNES_RIGHT_MASK;

		case 3: //Press A, Use Item
			return SNES_A_MASK;

		case 4: //Press B and A
			return SNES_B_MASK | SNES_A_MASK;

		case 5: //Press X, Flip Rear View
			return SNES_X_MASK;

		case 6: //Press B and X
			return SNES_B_MASK | SNES_X_MASK;

		case 7: //Press Left Trigger, Hop
			return SNES_TL_MASK;

		case 8: //Press B and Left Trigger
			return SNES_B_MASK | SNES_TL_MASK;

		case 9: //Press Right Trigger, Hop
			return SNES_TR_MASK;

		case 10: //Press B and Right Trigger
			return SNES_B_MASK | SNES_TR_MASK;

		case 11: //Press B, Left Trigger, Right Trigger, Power Slide
			return SNES_B_MASK | SNES_TL_MASK | SNES_TR_MASK;

		case 12: //Press B, Left Trigger, Right Trigger, Left dPad
			return SNES_B_MASK | SNES_TL_MASK | SNES_TR_MASK | SNES_LEFT_MASK;

		case 13: //Press B, Left Trigger, Right Trigger, Right dPad
			return SNES_B_MASK | SNES_TL_MASK | SNES_TR_MASK | SNES_RIGHT_MASK;

		case 14: //Press Y, Brake/Backwards
			return SNES_Y_MASK;

		case 15: //Press Y and Left dPad
			return SNES_Y_MASK | SNES_LEFT_MASK;

		case 16: //Press Y and Right dPad
			return SNES_Y_MASK | SNES_RIGHT_MASK;

		case 17: //Press Y and A
			return SNES_Y_MASK | SNES_A_MASK;

		case 18: //Press Y and X
			return SNES_Y_MASK | SNES_X_MASK;

		case 19: //Press Y and Left Trigger
			return SNES_Y_MASK | SNES_TL_MASK;

		case 20: //Press Y and Right Trigger
			return SNES_Y_MASK | SNES_TR_MASK;

		case 21: //No press
			return 0;

		default: //Also no press
			return 0;
		}
	}

}