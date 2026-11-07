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
	//Change of Coins
	uint8_t coinCount = rostrum[0x0E00];
	if (coinCount > coins) {
		for (int i = coins; i <= coinCount; i++) {
			cout << "AI Gained a Coin!" << endl;
			//myScore.collectedCoin();
		}
		coins = coinCount;
	}
	else if (coinCount < coins) {
		for (int i = coins; i >= coinCount; i--) {
			cout << "AI Lost a Coin!" << endl;
			myScore.lostCoin();
		}
		coins = coinCount;
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