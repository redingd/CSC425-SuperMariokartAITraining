#include "theMatrix.h"


using namespace std;


theMatrix::theMatrix() {
	//Set each layer to zeros
	layer1.assign(16 * 16, 0.0f); //Gemini Aided
	layer2.assign(8 * 8, 0.0f);
	layer3.assign(8 * 4, 0.0f);
	outputControl.assign(8 * 3, 0.0f);
	//Set each layer weight to 1/2
	toL1.assign(3 * 3, 0.5f);
	toL2.assign(3 * 3, 0.5f);
	toL3.assign(3 * 3, 0.5f);
	toOut.assign(3 * 3, 0.5f);
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
			float sum = 0.0f; //Could Have Bias For Whole Layer, Might Add as Variable Later
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
	auto itWhereMax = max_element(outputControl.begin(), outputControl.end());
	return distance(outputControl.begin(), itWhereMax);
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
	return choice();
}