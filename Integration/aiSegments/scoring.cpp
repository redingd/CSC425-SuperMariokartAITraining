#include "scoring.h"

using namespace std;


scoring::scoring() {
    score = 0;
    prevCheck = 0;
    pastPoints = 0;
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
void scoring::fellOffMap() {
    score -= 5;
}
void scoring::spunOut() {
    score -= 1;
}
/*void scoring::terrain(int type) { //Requires score to be a double
    if (type >= 20) {
        if (type >= 28) { //Normal Driving Area Bad Terrain
            score -= 0.005;
        }
        else { //Off Roading
            score -= 0.01;
        }
    }
}*/

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
void scoring::completedLap(int place) { //Place is Stored as (Place - 1) * 2
    score += 16 - place;
    checkPointCrossing(1); //For Reduction
    checkPointCrossing(1); //For Passing
}
/*void scoring::completedRace()*/
/*void scoring::failedRace()*/
void scoring::overallStandings(int points) { //Heavily Incentivises Passing a Race
    score += points * 10; //Better Placing in Earlier Races Increases Score Much More
    if (points - pastPoints == 9) { //Bonus for Placing First in this Race
        score += 100;
    }
    else if (points - pastPoints == 6) { //Bonus for Placing Second in this Race
        score += 50;
    }
    else if (points - pastPoints == 3) { //Bonus for Placing Third in this Race
        score += 20;
    }
    //Fourth Barely Helps to get to the Podium
    //Fifth and Below are not Passing
}

void scoring::collision() {
    score -= 1; //Might Change if Change to double
}