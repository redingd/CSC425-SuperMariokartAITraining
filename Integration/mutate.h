#pragma once //IMPORTANT TO NOT KEEP REDEFINING HEADER

#include<iostream>
#include<random>
#include<ctime>

using namespace std;

//Gemini aid is namespace mutate and inline
namespace mutate {

    inline double softMute = 0; //User Inputted
    inline double hardMute = 1;
    inline double mutationRate = 0;
    /*
    1 high, 0 low
    How often to actually mutate, so 1 at the beginning to get the ai going
    0.5 for most the time when training
    0 for letting it go and do its own best
    */
    inline mt19937 randomGenerator{time(0)};
    inline uniform_real_distribution<double> randomRange{0.0, 1.0};


    void setMutate(double);

    bool mutation(); //Called for mutations at every decision, true if ai mutates

}