#include "scoring.h"

using namespace std;


scoring::scoring() {
    score = 0;
    prevCheck = 0;
}


int scoring::getScore() {
    return score;
}
int scoring::seeScoreChange() {
    int temp = prevCheck;
    prevCheck = score;
    return score - temp;
}


/*void scoring::collectedCoin() {
    score += 2;
}*/
void scoring::lostCoin() {
    score -= 1;
}
/*void scoring::pickedItem() {
    score += 1;
}
void scoring::usedItem() {
    score += 1;
}*/
void scoring::hitByItem() {
    score -= 2;
}
void scoring::fellOffMap() {
    score -= 5;
}
void scoring::spunOut() {
    score -= 1;
}

void scoring::checkPointCrossing(int many) {
    if (many < 0) {
        score -= 11;
    }
    /* For Shortcuts
    else if (many > 1) {
        score += 20;
        for (int i = 0; i < many; i++) {
            score += 10;
        }
    }
    */
    else {
        score += 10;
    }
}
void scoring::completedLap(int place) {
    score += (10 - place) * 3;
}
/*void scoring::completedRace(unknown type) {
    score += not sure yet;
}*/

void scoring::collision() {
    score -= 1; //Might Change if Change to double
}