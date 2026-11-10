#pragma once //IMPORTANT TO NOT KEEP REDEFINING HEADER


class scoring {

private:

    int score; //Current Score
    int prevCheck; //Score when last checked by AI


public:

    scoring();

    int getScore();
    int seeScoreChange(); //Returns difference of the last time AI looked at its score

    //void collectedCoin();
    void lostCoin();
    //void pickedItem();
    //void usedItem();
    void hitByItem();
    void fellOffMap();
    void spunOut();

    void checkPointCrossing(int);
    void completedLap(int);
    //void completedRace(unknownType);

    void collision();

};