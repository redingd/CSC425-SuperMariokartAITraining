#pragma once //IMPORTANT TO NOT KEEP REDEFINING HEADER


class scoring {

private:
    int score = 0;

public:
    //void collectedCoin();
    void lostCoin();
    //void pickedItem();
    //void usedItem();
    void hitByItem();
    void fellOffMap();
    void spunOut();

    void completedLap(int);
    //void completedRace(unknownType);

};