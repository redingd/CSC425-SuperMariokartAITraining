#include "mutate.h"

using namespace std;

namespace mutate {//Gemini aided line

    void setMutate(double rate) {
        if (rate > 1 || rate < 0) {
            softMute = 0.5;
        }
        else {
            softMute = rate;
        }
        mutationRate = softMute * hardMute;
    }

    bool mutation() {
        if (randomRange(randomGenerator) <= mutationRate) {
            return true;
        }
        return false;
    }

    int pressThis() {
        //8 buttons are reasonable to press
        //It's reasonable to press two at once
        //8+7+6+5+4+3+2+1
        //36+1 for do nothing
        //37+1 for drift
        double ran = randomRange(randomGenerator);
        int outCombo = 38 - 38 * ran;
        return outCombo;
    }

}