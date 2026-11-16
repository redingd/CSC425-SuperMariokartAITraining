#pragma once //IMPORTANT TO NOT KEEP REDEFINING HEADER


class scoring {

private:

    int score; //Current Score
    int prevCheck; //Score when last checked by AI
    int pastPoints; //For Movement In Standings


public:

    scoring();

    int getScore();
    int seeScoreChange(); //Returns difference of the last time AI looked at its score

    //void collectedCoin();
    void lostCoin();
    //void pickedItem();
    //void usedItem();
    void fellOffMap();
    void spunOut(); //Any Version of Spinning Out
    //void terrain(int); //Requires score to be a double

    void checkPointCrossing(int);
    void completedLap(int);
    //void completedRace();
    //Call if Failed a Race (Does Not Affect Current Run Learning)
    //void failedRace();
    //Call if Passed a Race (Does Not Affect Current Run Learning)
    void overallStandings(int);

    void collision();

};