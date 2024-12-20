#pragma once

#include <string>
#include "candleStick.h"

class filterCandlestick
{
public:
    filterCandlestick(std::string country,
                      std::string startDate,
                      std::string endDate,
                      int startTemp,
                      int endTemp);

    // Default constructor
    filterCandlestick()
        : country(""), startDate(""), endDate(""), startTemp(0), endTemp(0) {}

    std::vector<candleStick> filterDataset(std::vector<std::vector<std::vector<std::string>>> datasets);

    static void printTable(std::vector<candleStick> vectorOfCandlesticks);

    std::string country;
    std::string startDate;
    std::string endDate;
    int startTemp;
    int endTemp;
};
