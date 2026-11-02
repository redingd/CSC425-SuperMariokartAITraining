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
        //22 Reasonable Single and Combo Button Presses
        double ran = randomRange(randomGenerator);
        int outCombo = 21 - 21 * ran;
        return outCombo;
    }

}