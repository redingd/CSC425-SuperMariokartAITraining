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

}