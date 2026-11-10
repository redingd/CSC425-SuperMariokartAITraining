#pragma once

#include<iostream>
#include <cstdint> //For uint8_t type

#include "scoring.h"
#include "../mutate.h" //temp

class AIgorithm {

private:
	
	int numInRun;
	scoring myScore; //may need to change to public, depends on if we can put the logic in ai or not
	bool single;

	//0 = Main Menu When First Starting, 127 = Opening of Race, 133 = End of Race
	int raceState = 0;
	//Active Race Values
	bool tooLongFlag = false;
	int lap = 0;
	int currentPoint = 0;
	int coins = 0;


public:

	AIgorithm(); //Default
	AIgorithm(int); //For numerous AI at once


	bool inARace(uint8_t*);
	//Add a Reset Function for When Race Begins or When Race Ends

	int aiButtonChoice();

	void updates(uint8_t*);
	void aiFeedback(); //Gemini mentioned
	bool endMe();


};