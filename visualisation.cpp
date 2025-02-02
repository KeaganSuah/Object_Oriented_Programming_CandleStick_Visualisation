#include "visualisation.h"
#include <iostream>

// Constructor
///// Code Written by myself to initialise the visualisation class. //////
visualisation::visualisation() {}

///// Code Written by myself to display candlesticks in chunks for better readability. //////
void visualisation::displayCandleSticks(const std::vector<candleStick> &vectorOfCandlesticks,
                                        const filterCandlestick &UserFiltered,
                                        unsigned int chunkSize)
{
    // Calculate the total number of candlesticks and chunks
    unsigned int totalCandlesticks = vectorOfCandlesticks.size();
    unsigned int numberOfChunks = (totalCandlesticks + chunkSize - 1) / chunkSize;

    for (unsigned int chunk = 0; chunk < numberOfChunks; ++chunk)
    {
        if (chunk == 0)
        {
            // Display the header for the first chunk
            displayHeader(UserFiltered);
            std::cout << "Candlestick Visualisation for Weather Data" << std::endl;
        }
        else
        {
            // Indicate continuation for subsequent chunks
            std::cout << "The visualisation continues below:" << std::endl
                      << std::endl;
        }

        // Determine the range of candlesticks to display in the current chunk
        unsigned int startIndex = chunk * chunkSize;
        unsigned int endIndex = (startIndex + chunkSize < totalCandlesticks) ? startIndex + chunkSize : totalCandlesticks;

        // Extract the current chunk of candlesticks
        std::vector<candleStick> currentChunk(vectorOfCandlesticks.begin() + startIndex, vectorOfCandlesticks.begin() + endIndex);

        // Display the chunk
        displayChunk(currentChunk, UserFiltered);
        displayTimestamps(currentChunk);
        std::cout << std::endl;
    }
}

///// Code Written by myself to display the header with user-defined filter information. //////
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

///// Code Written by myself to display a single chunk of candlestick data graphically. //////
void visualisation::displayChunk(const std::vector<candleStick> &chunk,
                                 const filterCandlestick &UserFiltered)
{
    // Start from the highest temperature in the user's filter
    int currentTemp = UserFiltered.endTemp;

    while (currentTemp >= UserFiltered.startTemp)
    {
        unsigned int blankCount = 0;

        // Add padding for temperature values
        while (blankCount < (4 - std::to_string(currentTemp).size()))
        {
            std::cout << " ";
            ++blankCount;
        }

        // Display the current temperature
        std::cout << currentTemp << ":  ";

        for (const candleStick &candleStick : chunk)
        {
            // Determine the minimum and maximum values of the candlestick
            double minVal = (candleStick.open < candleStick.close) ? candleStick.open : candleStick.close;
            double maxVal = (candleStick.open > candleStick.close) ? candleStick.open : candleStick.close;

            // Display graphical representation of the candlestick
            if (currentTemp <= candleStick.high && currentTemp > maxVal)
            {
                // Upper shadow
                std::cout << (candleStick.open > candleStick.close ? "\033[31m| \033[0m" : "\033[32m| \033[0m");
            }
            else if ((currentTemp <= maxVal && currentTemp >= minVal) ||
                     (std::round(maxVal) == std::round(minVal) && currentTemp == std::round(maxVal)))
            {
                // Candle body
                std::cout << (candleStick.open > candleStick.close ? "\033[31m█ \033[0m" : "\033[32m█ \033[0m");
            }
            else if (currentTemp < minVal && currentTemp > candleStick.low)
            {
                // Lower shadow
                std::cout << (candleStick.open > candleStick.close ? "\033[31m| \033[0m" : "\033[32m| \033[0m");
            }
            else
            {
                // Empty space
                std::cout << "· ";
            }
        }
        std::cout << std::endl;
        --currentTemp;
    }
}

///// Code Written by myself to display the timestamps of a chunk of candlestick data. //////
void visualisation::displayTimestamps(const std::vector<candleStick> &chunk)
{
    // Find the maximum length of timestamps in the chunk
    unsigned int maxLength = 0;
    for (const candleStick &candleStick : chunk)
    {
        if (candleStick.timestamp.size() > maxLength)
        {
            maxLength = candleStick.timestamp.size();
        }
    }

    // Display timestamps character by character for alignment
    for (unsigned int i = 0; i < maxLength; ++i)
    {
        std::cout << "       "; // Add spacing
        for (const candleStick &candleStick : chunk)
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

///// Code Written by myself to display candlestick data in tabular format. //////
void visualisation::printTable(std::vector<candleStick> vectorOfCandlesticks, filterCandlestick &UserFiltered)
{
    // Display the header with user filter information
    displayHeader(UserFiltered);

    // Print table heading
    std::cout << "Statistics Table for Weather Data" << std::endl;

    // Loop through the candlesticks and display each entry
    for (unsigned int i = 0; i < vectorOfCandlesticks.size(); ++i)
    {
        candleStick &entry = vectorOfCandlesticks[i];
        std::cout << "Country: " << candleStick::countryToString(entry.country) << ", "
                  << "Date: " << entry.timestamp << ", "
                  << "High: " << entry.high << ", "
                  << "Open: " << entry.open << ", "
                  << "Close: " << entry.close << ", "
                  << "Low: " << entry.low << std::endl;
    }
}
