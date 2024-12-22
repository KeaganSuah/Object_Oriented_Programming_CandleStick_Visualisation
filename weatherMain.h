#pragma once

#include "candleStick.h"
#include "filterCandlestick.h"
#include <vector>

///// Class for managing the main logic and user interface of the weather app. //////
class weatherMain
{
public:
    // Constructor to initialise the weatherMain class
    weatherMain();

    // Start the application and handle the main menu
    void init();

private:
    // Display the main menu
    void printMenu();

    // Display help information
    void printHelp();

    // Get and apply a filter from the user
    void getFilter(std::vector<std::vector<std::vector<std::string>>> datasets);

    // Show the statistics table for the filtered data
    void showTableStats();

    // Visualize the filtered data as candlesticks
    void showCandlestick();

    // Generate predictions for future data
    void getPrediction();

    // Get the user's choice from the menu
    int getUserOption();

    // Process the selected menu option
    void processUserOption(int userOption, std::vector<std::vector<std::vector<std::string>>> datasets);

    // Private variables to store filtered data and user filter settings
    std::vector<candleStick> vectorOfCandlesticks;
    filterCandlestick UserFiltered;
};
