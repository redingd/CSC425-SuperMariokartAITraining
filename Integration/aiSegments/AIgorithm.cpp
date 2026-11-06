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