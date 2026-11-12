#include "theMatrix.h"


using namespace std;


theMatrix::theMatrix() {
	//Set Selection History to Zero
	recentSelectionHistory.clear();
	//Set each layer to zeros
	layer1.assign(16 * 16, 0.0f); //Gemini Aided
	layer2.assign(8 * 8, 0.0f);
	layer3.assign(8 * 4, 0.0f);
	outputControl.assign(8 * 3, 0.0f);
	//Set Each Layer Weights
	toL1.resize(3 * 3);
	toL2.resize(3 * 3);
	toL3.resize(3 * 3);
	toOut.resize(3 * 3);
	for (int i = 0; i < toL1.size(); i++) {
		toL1[i] = weightRange(weightGenerator);
	}
	for (int i = 0; i < toL2.size(); i++) {
		toL2[i] = weightRange(weightGenerator);
	}
	for (int i = 0; i < toL3.size(); i++) {
		toL3[i] = weightRange(weightGenerator);
	}
	for (int i = 0; i < toOut.size(); i++) {
		toOut[i] = weightRange(weightGenerator);
	}
	//Set Bias for Each Button
	buttonBias.assign(8 * 3, weightRange(weightGenerator) * buttonBiasSize);
	//Forward Bias
	buttonBias[0] += 0.1 * buttonBiasSize;
	//Forward and Left Bias
	buttonBias[1] += 0.09 * buttonBiasSize;
	//Forward and Left Power Slide Bias
	buttonBias[10] += 0.081 * buttonBiasSize;
	//Backward Bias
	buttonBias[18] -= 0.081 * 0.9 * buttonBiasSize;
}

void theMatrix::vectorUpdateWeights(vector<float>& thisMatrix, float rate, int ranEle) {
	for (int i = 0; i < thisMatrix.size(); i++) {
		float change = 0.0;
		if (thisMatrix[i] < 0) {
			float rand = weightRange(weightGenerator);
			if (ranEle > 0) {
				change -= rate;
			}
			else if (ranEle < 0) {
				change += rate * rand / 10;
			}
			else {
				change -= rand / 100;
			}
		}
		else {
			float rand = weightRange(weightGenerator);
			if (ranEle > 0) {
				change += rate;
			}
			else if (ranEle < 0) {
				change -= rate * rand / 100;
			}
			else {
				change += rand / 10000;
			}
		}
		thisMatrix[i] += change;
		if (thisMatrix[i] > 5.0) {
			thisMatrix[i] = 5.0;
		}
		else if (thisMatrix[i] < -5.0) {
			thisMatrix[i] = -5.0;
		}
	}
}

//private methods

void theMatrix::convertTo16() { //Gemini Aided
	for (int rowOf16 = 0; rowOf16 < 16; rowOf16++) {
		for (int colOf16 = 0; colOf16 < 16; colOf16++) {
			float sum = 0.0f; //Could Have Bias For Whole Layer, Might Add as Variable Later
			int startRow = rowOf16 * 2 - 1;
			int startCol = colOf16 * 2 - 1;
			for (int extractorRow = 0; extractorRow < 3; extractorRow++) {
				for (int extractorCol = 0; extractorCol < 3; extractorCol++) {
					int rowOf32 = startRow + extractorRow;
					int colOf32 = startCol + extractorCol;
					//Check We're in 32 * 32
					if (rowOf32 >= 0 && rowOf32 < 32 && colOf32 >= 0 && colOf32 < 32) {
						float pixel = inputControl[rowOf32 * 32 + colOf32];
						float instantWeight = toL1[extractorRow * 3 + extractorCol];
						sum += pixel * instantWeight;
					}
				}
			}
			//Now Have a 16x16 Value
			layer1[rowOf16 * 16 + colOf16] = sum;
		}
	}
}

void theMatrix::convertTo8() {
	for (int rowOf8 = 0; rowOf8 < 8; rowOf8++) {
		for (int colOf8 = 0; colOf8 < 8; colOf8++) {
			float sum = 0.0f; //Could Have Bias For Whole Layer, Might Add as Variable Later
			int startRow = rowOf8 * 2 - 1;
			int startCol = colOf8 * 2 - 1;
			for (int extractorRow = 0; extractorRow < 3; extractorRow++) {
				for (int extractorCol = 0; extractorCol < 3; extractorCol++) {
					int rowOf16 = startRow + extractorRow;
					int colOf16 = startCol + extractorCol;
					//Check We're in 16x16
					if (rowOf16 >= 0 && rowOf16 < 16 && colOf16 >= 0 && colOf16 < 16) {
						float pixel = layer1[rowOf16 * 16 + colOf16];
						float instantWeight = toL2[extractorRow * 3 + extractorCol];
						sum += pixel * instantWeight;
					}
				}
			}
			//Now Have an 8x8 Value
			layer2[rowOf8 * 8 + colOf8] = sum;
		}
	}
}

void theMatrix::convertTo4() {
	for (int rowOf84 = 0; rowOf84 < 8; rowOf84++) {
		for (int colOf4 = 0; colOf4 < 4; colOf4++) {
			float sum = 0.0f; //Could Have Bias For Whole Layer, Might Add as Variable Later
			int startRow = rowOf84 - 1; //Number of Rows Doesn't Change
			int startCol = colOf4 * 2 - 1;
			for (int extractorRow = 0; extractorRow < 3; extractorRow++) {
				for (int extractorCol = 0; extractorCol < 3; extractorCol++) {
					int rowOf8 = startRow + extractorRow;
					int colOf8 = startCol + extractorCol;
					//Check We're in 8X8
					if (rowOf8 >= 0 && rowOf8 < 8 && colOf8 >= 0 && colOf8 < 8) {
						float pixel = layer2[rowOf8 * 8 + colOf8];
						float instantWeight = toL3[extractorRow * 3 + extractorCol];
						sum += pixel * instantWeight;
					}
				}
			}
			//Now Have an 8x4 Value
			layer3[rowOf84 * 4 + colOf4] = sum; //Multiply by Width/Num Columns
		}
	}
}

void theMatrix::convertTo3() {
	for (int rowOf83 = 0; rowOf83 < 8; rowOf83++) {
		for (int colOf3 = 0; colOf3 < 3; colOf3++) {
			float sum = buttonBias[rowOf83 * 3 + colOf3]; //Add Bias For Specific Buttons
			int startRow = rowOf83 - 1; //Number of Rows Doesn't Change
			int startCol = colOf3;		//Gemini said no - 1
			for (int extractorRow = 0; extractorRow < 3; extractorRow++) {
				for (int extractorCol = 0; extractorCol < 3; extractorCol++) {
					int rowOf84 = startRow + extractorRow;
					int colOf4 = startCol + extractorCol;
					//Check We're in 8X4
					if (rowOf84 >= 0 && rowOf84 < 8 && colOf4 >= 0 && colOf4 < 4) {
						float pixel = layer3[rowOf84 * 4 + colOf4];
						float instantWeight = toOut[extractorRow * 3 + extractorCol];
						sum += pixel * instantWeight;
					}
				}
			}
			//Now Have an 8x3 Value
			outputControl[rowOf83 * 3 + colOf3] = sum; //Multiply by Width/Num Columns
		}
	}
}

int theMatrix::choice() {
	auto itWhereMax = max_element(outputControl.begin(), outputControl.end()); //Gemini Aided
	return distance(outputControl.begin(), itWhereMax); //Gemini Aided
}


//public methods

int theMatrix::process(const vector<float>& screen) {
	if (screen.size() != 1024) {
		cout << endl << "Recieved Wrong Screen Size!" << endl;
		return -1;
	}
	inputControl = screen;
	convertTo16();
	convertTo8();
	convertTo4();
	convertTo3();
	int selection = choice();
	recentSelectionHistory.push_back(selection);
	return selection;
}

void theMatrix::updateWeights(int primary, double secondary) {
	float reinforcementRate = (float) secondary / 10;
	int selectionCount[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	if (recentSelectionHistory.empty()) {}
	else {
		for (auto it = recentSelectionHistory.begin(); it != recentSelectionHistory.end(); ++it) {
			selectionCount[*it]++;
		}
	}
	if (primary > 0) { //Reinforce Learning
		vectorUpdateWeights(toL1, reinforcementRate, 1);
		vectorUpdateWeights(toL2, reinforcementRate, 1);
		vectorUpdateWeights(toL3, reinforcementRate, 1);
		vectorUpdateWeights(toOut, reinforcementRate, 1);
	}
	else if (primary < 0) { //Uninforce Learning
		vectorUpdateWeights(toL1, reinforcementRate, -1);
		vectorUpdateWeights(toL2, reinforcementRate, -1);
		vectorUpdateWeights(toL3, reinforcementRate, -1);
		vectorUpdateWeights(toOut, reinforcementRate, -1);
	}
	else { //Tiny Rando
		vectorUpdateWeights(toL1, reinforcementRate, 0);
		vectorUpdateWeights(toL2, reinforcementRate, 0);
		vectorUpdateWeights(toL3, reinforcementRate, 0);
		vectorUpdateWeights(toOut, reinforcementRate, 0);
	}
	//Reset Selection History
	recentSelectionHistory.clear();
}

string theMatrix::getBiasWeights() {
	string result = "\nLayer 1: ";
	for (int i = 0; i < toL1.size(); i++) {
		result += to_string(toL1[i]);
		if (i < toL1.size() - 1) {
			result += ", ";
		}
		else {
			result += "\n";
		}
	}
	result += "Layer 2: ";
	for (int i = 0; i < toL2.size(); i++) {
		result += to_string(toL2[i]);
		if (i < toL2.size() - 1) {
			result += ", ";
		}
		else {
			result += "\n";
		}
	}
	result += "Layer 3: ";
	for (int i = 0; i < toL3.size(); i++) {
		result += to_string(toL3[i]);
		if (i < toL3.size() - 1) {
			result += ", ";
		}
		else {
			result += "\n";
		}
	}
	result += "Out Layer: ";
	for (int i = 0; i < toOut.size(); i++) {
		result += to_string(toOut[i]);
		if (i < toOut.size() - 1) {
			result += ", ";
		}
		else {
			result += "\n";
		}
	}
	result += "Button Bias: ";
	for (int i = 0; i < buttonBias.size(); i++) {
		result += to_string(buttonBias[i]);
		if (i < buttonBias.size() - 1) {
			result += ", ";
		}
		else {
			result += "\n";
		}
	}
	return result;
}