#include "visualisation.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

visualisation::visualisation() {}

void visualisation::displayChunks(const std::vector<candleStick> &vectorOfCandlesticks,
                                  const filterCandlestick &UserFiltered,
                                  size_t chunkSize)
{
    size_t totalCandlesticks = vectorOfCandlesticks.size();
    size_t numberOfChunks = (totalCandlesticks + chunkSize - 1) / chunkSize;

    for (size_t chunk = 0; chunk < numberOfChunks; ++chunk)
    {
        if (chunk == 0)
        {
            displayHeader(UserFiltered);
        }
        else
        {
            std::cout << "The visualisation continues below:" << std::endl
                      << std::endl;
        }

        size_t startIndex = chunk * chunkSize;
        size_t endIndex = std::min(startIndex + chunkSize, totalCandlesticks);
        std::vector<candleStick> currentChunk(vectorOfCandlesticks.begin() + startIndex, vectorOfCandlesticks.begin() + endIndex);

        displayChunk(currentChunk, UserFiltered);
        displayTimestamps(currentChunk);
        std::cout << std::endl;
    }
}

void visualisation::displayHeader(const filterCandlestick &UserFiltered)
{
    std::cout << std::endl
              << "<<     Weather in - " << UserFiltered.country
              << " - During the period from " << UserFiltered.startDate
              << " to " << UserFiltered.endDate
              << " with a range from " << UserFiltered.startTemp
              << "℃ to " << UserFiltered.endTemp
              << "℃.     >>" << std::endl
              << std::endl;
}

void visualisation::displayChunk(const std::vector<candleStick> &chunk,
                                 const filterCandlestick &UserFiltered)
{
    int currentStartTemp = UserFiltered.startTemp;

    while (currentStartTemp >= UserFiltered.endTemp)
    {
        std::cout << std::setw(4) << currentStartTemp << ":  ";
        for (const auto &candleStick : chunk)
        {
            double minVal = std::min(candleStick.open, candleStick.close);
            double maxVal = std::max(candleStick.open, candleStick.close);

            if (currentStartTemp <= candleStick.high && currentStartTemp > maxVal)
            {
                std::cout << (candleStick.open > candleStick.close ? "\033[32m| \033[0m" : "\033[31m| \033[0m");
            }
            else if (currentStartTemp <= maxVal && currentStartTemp >= minVal ||
                     (static_cast<int>(maxVal) == static_cast<int>(minVal) && currentStartTemp == static_cast<int>(maxVal)))
            {
                std::cout << (candleStick.open > candleStick.close ? "\033[32m█ \033[0m" : "\033[31m█ \033[0m");
            }
            else if (currentStartTemp < minVal && currentStartTemp > candleStick.low)
            {
                std::cout << (candleStick.open > candleStick.close ? "\033[32m| \033[0m" : "\033[31m| \033[0m");
            }
            else
            {
                std::cout << "· ";
            }
        }
        std::cout << std::endl;
        --currentStartTemp;
    }
}

void visualisation::displayTimestamps(const std::vector<candleStick> &chunk)
{
    size_t maxLength = 0;
    for (const auto &candleStick : chunk)
    {
        maxLength = std::max(maxLength, candleStick.timestamp.size());
    }

    for (size_t i = 0; i < maxLength; ++i)
    {
        std::cout << "       "; // 6 spaces
        for (const auto &candleStick : chunk)
        {
            if (i < candleStick.timestamp.size())
            {
                std::cout << candleStick.timestamp[i] << " ";
            }
            else
            {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}
