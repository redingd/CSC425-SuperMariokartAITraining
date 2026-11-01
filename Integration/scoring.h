#pragma once //IMPORTANT TO NOT KEEP REDEFINING HEADER

using namespace std;

namespace scoring {

    inline int score = 0;

    //void collectedCoin();
    void lostCoin();
    //void pickedItem();
    //void usedItem();
    void hitByItem();
    void fellOffMap();
    void spunOut();

    void completedLap(int);
    //void completedRace(unknownType);

}