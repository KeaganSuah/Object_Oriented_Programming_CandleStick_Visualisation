#pragma once

#include <vector>

class weatherMain
{
public:
    weatherMain();
    /** Call this to start the sim */
    void init();

private:
    void printMenu();
    void printHelp();
    void getFilter();
    void showTableStats();
    void showCandlestick();
    void getPrediction();
    int getUserOption();
    void processUserOption(int userOption);
};
