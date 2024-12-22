#pragma once

#include <vector>
#include <string>
#include "candleStick.h"
#include "filterCandlestick.h"

///// Class for visualising weather data as candlesticks. //////
class visualisation
{
public:
    // Constructor to initialise the visualisation class
    visualisation();

    // Display candlesticks graphically in chunks
    static void displayCandleSticks(const std::vector<candleStick> &vectorOfCandlesticks,
                                    const filterCandlestick &UserFiltered,
                                    unsigned int chunkSize = 40);

    // Display a table of candlestick data
    static void printTable(std::vector<candleStick> vectorOfCandlesticks, filterCandlestick &UserFiltered);

private:
    // Display the header with user filter information
    static void displayHeader(const filterCandlestick &UserFiltered);

    // Display a single chunk of candlestick data
    static void displayChunk(const std::vector<candleStick> &chunk,
                             const filterCandlestick &UserFiltered);

    // Display the timestamps of a chunk of candlestick data
    static void displayTimestamps(const std::vector<candleStick> &chunk);
};
