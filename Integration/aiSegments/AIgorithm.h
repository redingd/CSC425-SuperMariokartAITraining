#pragma once

#include <iostream>
#include <vector>
#include <cstdint> //For uint8_t type

#include "scoring.h"
#include "theMatrix.h"

class AIgorithm {

private:
	
	int numInRun;
	bool single;
	theMatrix myNueralNetwork = theMatrix();
	scoring myScore = scoring(); //may need to change to public, depends on if we can put the logic in ai or not

	//For Feedback Updating
	bool updateFeedFlag = false;
	int scoreChange = 0;
	double scoringAsecond = 0;

	//0 = Main Menu When First Starting, 127 = Opening of Race, 133 = End of Race
	int raceState = 0;
	//Active Race Values
	bool tooLongFlag = false;
	int lap = 0;
	int myCheckPoint = 0;
	int coins = 0;
	int prevRaceScore = 0;


public:

	AIgorithm(); //Default
	AIgorithm(int); //For Numerous AI at Once


	bool inARace(uint8_t*);
	//Add a Reset Function for When Race Begins or When Race Ends

	int aiButtonChoice(const std::vector<float>&);

	void updates(uint8_t*);
	void raceReset(uint8_t*);
	void aiFeedback(uint8_t*); //Gemini mentioned
	bool endMe();


};