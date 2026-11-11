#include "AIgorithm.h"

using namespace std;


AIgorithm::AIgorithm() {
	numInRun = 0;
	single = true;
}

AIgorithm::AIgorithm(int num) {
	numInRun = num;
	single = false;
}


bool AIgorithm::inARace(uint8_t* raceFinder) {
	int currentState = raceFinder[0x10C1]; //For raceState Change
	if (currentState != raceState) { //Has Race State Changed
		if (raceState == 0 || raceState == 133) { //When Race Begins
			raceReset(raceFinder);
			if (single) {
				cout << "AIgorithm is in a Race!" << endl;
			}
			else {
				cout << "I'm AI " << numInRun <<
					" and I'm in a Race!" << endl;
			}
			raceState = currentState;
			return true;
		}
		else if (raceState == 132) { //When Race Ends
			raceState = currentState;
			//Need Update Lap Function
			return false;
		}
		raceState = currentState;
		//Need Update Lap Function
	}
	return !(raceState == 0 || raceState == 133); //Not Open Menu or Autopilot
}


int AIgorithm::aiButtonChoice() {
	return mutate::pressThis();
}


void AIgorithm::updates(uint8_t* rostrum) {
	uint8_t minutes = rostrum[0x0104];
	uint8_t seconds = rostrum[0x0102];
	uint8_t smallerTimerB = rostrum[0x0101];
	uint8_t smallerTimerA = rostrum[0x0100];
	//Active Racing Check - Looks For Running Clock
	bool racing = minutes > 0 || seconds > 0 || smallerTimerB > 0 || smallerTimerA > 0;
	if (racing) {
		//Took Too Long Check
		if (smallerTimerA == 153 && smallerTimerB == 153 && seconds == 89 && minutes == 9) {
			tooLongFlag = true;
		}

		//Change of Coins
		uint8_t coinCount = rostrum[0x0E00]; //Gemini
		if (coinCount > coins) {
			for (int i = coins; i < coinCount; i++) {
				if (single) {
					cout << "AIgorithm Gained a Coin!" << endl;
				}
				else {
					cout << "I'm AI " << numInRun <<
						" and I gained a coin!" << endl;
				}
				//myScore.collectedCoin();
			}
			coins = coinCount;
		}
		else if (coinCount < coins) {
			for (int i = coins; i > coinCount; i--) {
				if (single) {
					cout << "AIgorithm Lost a Coin!" << endl;
				}
				else {
					cout << "I'm AI " << numInRun <<
						" and I lost a coin!" << endl;
				}
				myScore.lostCoin();
			}
			coins = coinCount;
		}

		//Crossing a Checkpoint
		uint8_t currentCheckPoint = rostrum[0x10C0];
		if (currentCheckPoint != myCheckPoint) {
			if (currentCheckPoint - myCheckPoint < -20) { //Check for Crossing Start
				myScore.checkPointCrossing(1);
			}
			else {
				myScore.checkPointCrossing(currentCheckPoint - myCheckPoint);
			}
			myCheckPoint = currentCheckPoint;
		}
	}
}

void AIgorithm::raceReset(uint8_t* sheep) {
	coins = 0;
	lap = 0;
	myCheckPoint = sheep[0x10C0];
	prevRaceScore = myScore.getScore();
}

void AIgorithm::aiFeedback(uint8_t* timer) {
	uint8_t minutes = timer[0x0104];
	uint8_t untrueSeconds = timer[0x0102]; //Every tenth second counts as 16 instead of 10 (1 is 1 ... 9 is 9)
	int trueSeconds = untrueSeconds / 16 * 10 + untrueSeconds % 16;
	int howsGoin = myScore.seeScoreChange();
	int elapsedSec = trueSeconds + minutes * 60;
	if (elapsedSec == 0) {
		elapsedSec = 1;
	}
	double scorePerSec = (double) (myScore.getScore() - prevRaceScore) / (double) elapsedSec;
	if (single) {
		cout << "AIgorithm Score: " << myScore.getScore() << "\t Scoring/Second: " << scorePerSec << endl;
	}
	else {
		cout << "I'm AI " << numInRun <<
			" and my current score is: " << myScore.getScore() << "\t and scoring/second is: " << scorePerSec << endl;
	}
	updateFeedFlag = true;
	scoreChange = howsGoin;
	scoringAsecond = scorePerSec;
}

bool AIgorithm::endMe() {
	if (tooLongFlag) {
		if (single) {
			cout << "The AIgorithm Took Too Long" << endl;
		}
		else {
			cout << "I'm AI " << numInRun <<
				"and I took too long" << endl;
		}
		return true;
	}
	return false;
}