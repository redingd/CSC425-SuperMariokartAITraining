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

	//Active Race Values
	bool tooLongFlag = false;
	int speed;
	int coins = 0;


public:

	AIgorithm(); //Default
	AIgorithm(int); //For numerous AI at once


	int aiButtonChoice();

	void updates(uint8_t*);
	void aiFeedback(); //Gemini mentioned
	bool endMe();


};