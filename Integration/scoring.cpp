#include "scoring.h"

using namespace std;


    void scoring::collectedCoin() {
        score += 2;
    }
    void scoring::lostCoin() {
        score -= 1;
    }
    void scoring::pickedItem() {
        score += 1;
    }
    void scoring::usedItem() {
        score += 1;
    }
    void scoring::hitByItem() {
        score -= 2;
    }
    void scoring::fellOffMap() {
        score -= 5;
    }
    void scoring::spunOut() {
        score -= 1;
    }

    void scoring::completedLap(int place) {
        score += (10 - place) * 3;
    }
