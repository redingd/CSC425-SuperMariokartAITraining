#pragma once //IMPORTANT TO NOT KEEP REDEFINING HEADER

using namespace std;

class scoring {

    public:
    int score;

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