#include "filterCandlestick.h"
#include <vector>
#include <iostream>
#include <limits>

// Constructor
///// Code Written by myself to initialise the filterCandlestick object. //////
filterCandlestick::filterCandlestick(std::string _country,
                                     std::string _startDate,
                                     std::string _endDate,
                                     int _startTemp,
                                     int _endTemp)
    : country(_country),
      startDate(_startDate),
      endDate(_endDate),
      startTemp(_startTemp),
      endTemp(_endTemp) {}

///// Code Written by myself to calculate statistical data for a specific date and country. //////
void filterCandlestick::calculateStats(const std::vector<std::vector<std::string>> &minData,
                                       const std::vector<std::vector<std::string>> &maxData,
                                       const std::vector<std::vector<std::string>> &meanData,
                                       int country,
                                       unsigned int date,
                                       double &totalMin,
                                       double &totalMax,
                                       double &totalMean,
                                       int &totalEntries)
{
    try
    {
        // Extract numerical data for the specified country and date
        double minValue = std::stod(minData[date][country]);
        double maxValue = std::stod(maxData[date][country]);
        double meanValue = std::stod(meanData[date][country]);

        // Update the running totals for min, max, and mean
        totalMin = std::min(totalMin, minValue);
        totalMax = std::max(totalMax, maxValue);
        totalMean += meanValue;
        totalEntries++;
    }
    catch (const std::exception &e)
    {
        // Handle potential conversion errors or out-of-bounds access
        std::cerr << "Error processing row " << date << ": " << e.what() << std::endl;
    }
}

///// Code Written by myself to determine granularity and process data accordingly. //////
void filterCandlestick::processData(const std::vector<std::vector<std::string>> &minData,
                                    const std::vector<std::vector<std::string>> &maxData,
                                    const std::vector<std::vector<std::string>> &meanData,
                                    std::vector<candleStick> &vectorOfCandlesticks,
                                    int country,
                                    const std::string &startPeriod,
                                    const std::string &endPeriod,
                                    const std::string &granularity)
{
    // Determine the substring length based on granularity
    unsigned int substringLength = (granularity == "year") ? 4 : (granularity == "month") ? 7
                                                                                          : 10;

    double totalMin = std::numeric_limits<double>::max();
    double totalMax = std::numeric_limits<double>::lowest();
    double totalMean = 0.0;
    double previousMean = 0.0;
    int totalEntries = 0;
    std::string currentPeriod = "";

    // Calculate the previous mean value before the start period
    for (unsigned int date = 0; date < minData.size(); ++date)
    {
        std::string period = minData[date][0].substr(0, substringLength);
        if (period >= startPeriod)
        {
            break;
        }
        previousMean = std::stod(meanData[date][country]);
    }

    // Process data and aggregate statistics
    for (unsigned int date = 0; date < minData.size(); ++date)
    {
        std::string period = minData[date][0].substr(0, substringLength);

        if (period < startPeriod)
        {
            continue;
        }
        if (period > endPeriod)
        {
            break;
        }

        if (currentPeriod.empty())
        {
            currentPeriod = period;
        }
        else if (period != currentPeriod)
        {
            // Calculate the mean for the current period and store the data
            totalMean /= totalEntries;
            storeToCandleStick(currentPeriod, totalMean, totalMax, totalMin, previousMean, country, vectorOfCandlesticks);

            // Reset values for the next period
            previousMean = totalMean;
            currentPeriod = period;
            totalMin = std::numeric_limits<double>::max();
            totalMax = std::numeric_limits<double>::lowest();
            totalMean = 0.0;
            totalEntries = 0;
        }

        // Update statistics for the current date
        calculateStats(minData, maxData, meanData, country, date, totalMin, totalMax, totalMean, totalEntries);
    }

    // Store data for the last period
    if (totalEntries > 0)
    {
        totalMean /= totalEntries;
        storeToCandleStick(currentPeriod, totalMean, totalMax, totalMin, previousMean, country, vectorOfCandlesticks);
    }
}

///// Code Written by myself to determine which format to process. //////
void filterCandlestick::processFormat(const std::vector<std::vector<std::string>> &minData,
                                      const std::vector<std::vector<std::string>> &maxData,
                                      const std::vector<std::vector<std::string>> &meanData,
                                      std::vector<candleStick> &vectorOfCandlesticks,
                                      int country,
                                      const std::string &startPeriod,
                                      const std::string &endPeriod)
{
    // yyyy format
    if (startPeriod.size() == 4 && endPeriod.size() == 4)
    {
        processData(minData, maxData, meanData, vectorOfCandlesticks, country, startPeriod, endPeriod, "year");
    }
    // yyyy-mm format
    else if (startPeriod.size() == 7 && endPeriod.size() == 7)
    {
        processData(minData, maxData, meanData, vectorOfCandlesticks, country, startPeriod, endPeriod, "month");
    }
    // yyyy-mm-dd format
    else if (startPeriod.size() == 10 && endPeriod.size() == 10)
    {
        processData(minData, maxData, meanData, vectorOfCandlesticks, country, startPeriod, endPeriod, "day");
    }
}

///// Code Written by myself to store aggregated data into a candleStick object. //////
void filterCandlestick::storeToCandleStick(const std::string &period,
                                           double totalMean,
                                           double totalMax,
                                           double totalMin,
                                           double previousMean,
                                           int country,
                                           std::vector<candleStick> &vectorOfCandlesticks)
{
    try
    {
        // Create a new candleStick object and add it to the vector
        candleStick weatherData{
            period,
            previousMean,
            totalMax,
            totalMin,
            totalMean,
            candleStick::intToCountryType(country)};

        vectorOfCandlesticks.push_back(weatherData);
    }
    catch (const std::exception &e)
    {
        // Handle any exceptions that may occur during object creation
        std::cerr << "Error processing period " << period << ": " << e.what() << std::endl;
    }
}

///// Code Written by myself to filter the dataset based on user input. //////
std::vector<candleStick> filterCandlestick::filterDataset(std::vector<std::vector<std::vector<std::string>>> datasets)
{
    std::vector<candleStick> vectorOfCandlesticks;

    // Extract datasets for min, max, and mean values
    std::vector<std::vector<std::string>> minData = datasets[0];
    std::vector<std::vector<std::string>> maxData = datasets[1];
    std::vector<std::vector<std::string>> meanData = datasets[2];

    // Convert the country string to an integer code
    int country = candleStick::stringToCountryInteger(this->country);

    // Determine the range to process
    std::string startPeriod = this->startDate;
    std::string endPeriod = this->endDate;

    // Process the data based on the input format
    processFormat(minData, maxData, meanData, vectorOfCandlesticks, country, startPeriod, endPeriod);

    return vectorOfCandlesticks;
}
