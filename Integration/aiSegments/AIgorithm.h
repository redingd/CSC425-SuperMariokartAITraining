#pragma once

#include<iostream>

#include "scoring.h"
#include "../mutate.h" //temp

class AIgorithm {

private:
	
	int numInRun;
	scoring myScore; //may need to change to public, depends on if we can put the logic in ai or not
	bool single;


public:

	AIgorithm(); //Default
	AIgorithm(int); //For numerous AI at once


	int aiButtonChoice();

	void aiFeedback(); //Gemini mentioned

};