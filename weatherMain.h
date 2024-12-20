#pragma once

#include "candleStick.h"
#include "filterCandlestick.h"
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
    void getFilter(std::vector<std::vector<std::vector<std::string>>> datasets);
    void showTableStats();
    void showCandlestick();
    void getPrediction();
    int getUserOption();
    void processUserOption(int userOption, std::vector<std::vector<std::vector<std::string>>> datasets);

    // Private Variables
    std::vector<candleStick> vectorOfCandlesticks;
    filterCandlestick UserFiltered;
};
