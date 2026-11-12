#pragma once

#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
//For Mistakes and No Prev Data
#include<random>
#include<ctime>

class theMatrix {

private:

	std::vector<float> inputControl; //32x32 Matrix

	//Might Want to change 8x4 to 6x6 and 8x3 to 6x4
	//Weight Vectors are 3x3
	//Might Need Different Weight Vectors for Different Races
	std::vector<float> toL1;	//32 to 16 Weights
	std::vector<float> layer1;	//16x16 Matrix
	std::vector<float> toL2;	//16 to 8 Weights
	std::vector<float> layer2;	//8x8 Matrix
	std::vector<float> toL3;	//8 to 4 Weights
	std::vector<float> layer3;	//8x4 Matrix

	std::vector<float> toOut;			//4 to 3 Weights
	std::vector<float> buttonBias;		//Set a Bias to Which Buttons to Press (Would be Something a Person Knows)
	std::vector<float> outputControl;	//8x3 Matrix


	std::mt19937 weightGenerator{ (unsigned int)time(0) };
	std::uniform_real_distribution<float> weightRange{ -0.5, 0.5 };


	void convertTo16(); //Conversion From 32x32 to 16x16
	void convertTo8();	//Conversion From 16x16 to 8x8
	void convertTo4();	//Conversion From 8x8 to 8x4
	void convertTo3();	//Conversion From 8x4 to 8x3

	int choice();

public:

	theMatrix();

	int process(const std::vector<float>&);
	void updateWeights(int, double);
	std::string getBiasWeights();

};