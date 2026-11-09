#include "AIgorithm.h"

using namespace std;


AIgorithm::AIgorithm() {
	myScore = scoring();
	numInRun = 0;
	single = true;
}

AIgorithm::AIgorithm(int num) {
	myScore = scoring();
	numInRun = num;
	single = false;
}


int AIgorithm::aiButtonChoice() {
	return mutate::pressThis();
}


void AIgorithm::updates(uint8_t* rostrum) {
	uint8_t minutes = rostrum[0x0104];
	uint8_t seconds = rostrum[0x0102];
	uint8_t smallerTimerB = rostrum[0x0101];
	uint8_t smallerTimerA = rostrum[0x0100];
	//Temporary Racing Check - Will Need Updated When More than One Race
	bool racing = minutes > 0 || seconds > 0 || smallerTimerB > 0 || smallerTimerA > 0;
	if (racing) {
		//Took Too Long Check
		if (smallerTimerA == 153 && smallerTimerB == 153 && seconds == 89 && minutes == 9) {
			tooLongFlag = true;
		}

		//Change of Coins
		uint8_t coinCount = rostrum[0x0E00];
		if (coinCount > coins) {
			for (int i = coins; i < coinCount; i++) {
				cout << "AI Gained a Coin!" << endl;
				//myScore.collectedCoin();
			}
			coins = coinCount;
			cout << "AI Coin Count: " << coins << endl;
		}
		else if (coinCount < coins) {
			for (int i = coins; i > coinCount; i--) {
				cout << "AI Lost a Coin!" << endl;
				myScore.lostCoin();
			}
			coins = coinCount;
			cout << "AI Coin Count: " << coins << endl;
		}
	}
	else {
		coins = 0;
	}
}

void AIgorithm::aiFeedback() {
	int howsGoin = myScore.seeScoreChange();
	if (single) {
		cout << "Current Score: " << myScore.getScore() << endl;
	}
	else {
		cout << "I'm AI " << numInRun <<
			" and my current score is: " << myScore.getScore() << endl;
	}
}

bool AIgorithm::endMe() {
	if (tooLongFlag) {
		cout << "The AIgorithm Took Too Long" << endl;
		return true;
	}
	return false;
}