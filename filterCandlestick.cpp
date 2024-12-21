#include "filterCandlestick.h"
#include <vector>
#include <iostream>

// Constructor
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

// Helper function to calculate min, max, and mean
void filterCandlestick::calculateStats(const std::vector<std::vector<std::string>> &minData,
                                       const std::vector<std::vector<std::string>> &maxData,
                                       const std::vector<std::vector<std::string>> &meanData,
                                       int country,
                                       size_t date,
                                       double &totalMin,
                                       double &totalMax,
                                       double &totalMean,
                                       int &totalEntries)
{
    try
    {
        double minValue = std::stod(minData[date][country]);
        double maxValue = std::stod(maxData[date][country]);
        double meanValue = std::stod(meanData[date][country]);

        totalMin = std::min(totalMin, minValue);
        totalMax = std::max(totalMax, maxValue);
        totalMean += meanValue;
        totalEntries++;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error processing row " << date << ": " << e.what() << std::endl;
    }
}

// Helper function to determine which format to process
void filterCandlestick::processFormat(const std::vector<std::vector<std::string>> &minData,
                                      const std::vector<std::vector<std::string>> &maxData,
                                      const std::vector<std::vector<std::string>> &meanData,
                                      std::vector<candleStick> &vectorOfCandlesticks,
                                      int country,
                                      const std::string &startPeriod,
                                      const std::string &endPeriod)
{
    if (startPeriod.size() == 4 && endPeriod.size() == 4) // yyyy format
    {
        processYearlyFormat(minData, maxData, meanData, vectorOfCandlesticks, country, startPeriod, endPeriod);
    }
    else if (startPeriod.size() == 7 && endPeriod.size() == 7) // yyyy-mm format
    {
        processMonthlyFormat(minData, maxData, meanData, vectorOfCandlesticks, country, startPeriod, endPeriod);
    }
    else if (startPeriod.size() == 10 && endPeriod.size() == 10) // yyyy-mm-dd format
    {
        processDailyFormat(minData, maxData, meanData, vectorOfCandlesticks, country, startPeriod, endPeriod);
    }
}

// Helper function to store data into candleStick object
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
        candleStick weatherData{
            period,
            totalMean,
            totalMax,
            totalMin,
            previousMean,
            candleStick::intToCountryType(country)};

        vectorOfCandlesticks.push_back(weatherData);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error processing period " << period << ": " << e.what() << std::endl;
    }
}

// Function to process yearly format
void filterCandlestick::processYearlyFormat(const std::vector<std::vector<std::string>> &minData,
                                            const std::vector<std::vector<std::string>> &maxData,
                                            const std::vector<std::vector<std::string>> &meanData,
                                            std::vector<candleStick> &vectorOfCandlesticks,
                                            int country,
                                            const std::string &startPeriod,
                                            const std::string &endPeriod)
{
    double totalMin = std::numeric_limits<double>::max();
    double totalMax = std::numeric_limits<double>::lowest();
    double totalMean = 0.0;
    double previousMean = 0.0;
    int totalEntries = 0;
    std::string currentYear = "";

    // Initialize previousMean
    for (size_t date = 0; date < minData.size(); ++date)
    {
        std::string year = minData[date][0].substr(0, 4);
        if (year >= startPeriod)
        {
            break;
        }
        previousMean = std::stod(meanData[date][country]);
    }

    for (size_t date = 0; date < minData.size(); ++date)
    {
        std::string year = minData[date][0].substr(0, 4);

        if (year < startPeriod)
        {
            continue;
        }
        if (year > endPeriod)
        {
            break;
        }

        if (currentYear.empty())
        {
            currentYear = year;
        }
        else if (year != currentYear)
        {
            totalMean /= totalEntries;
            storeToCandleStick(currentYear, totalMean, totalMax, totalMin, previousMean, country, vectorOfCandlesticks);

            previousMean = totalMean;
            currentYear = year;
            totalMin = std::numeric_limits<double>::max();
            totalMax = std::numeric_limits<double>::lowest();
            totalMean = 0.0;
            totalEntries = 0;
        }

        calculateStats(minData, maxData, meanData, country, date, totalMin, totalMax, totalMean, totalEntries);
    }

    if (totalEntries > 0)
    {
        totalMean /= totalEntries;
        storeToCandleStick(currentYear, totalMean, totalMax, totalMin, previousMean, country, vectorOfCandlesticks);
    }
}

// Function to process monthly format
void filterCandlestick::processMonthlyFormat(const std::vector<std::vector<std::string>> &minData,
                                             const std::vector<std::vector<std::string>> &maxData,
                                             const std::vector<std::vector<std::string>> &meanData,
                                             std::vector<candleStick> &vectorOfCandlesticks,
                                             int country,
                                             const std::string &startPeriod,
                                             const std::string &endPeriod)
{
    double totalMin = std::numeric_limits<double>::max();
    double totalMax = std::numeric_limits<double>::lowest();
    double totalMean = 0.0;
    double previousMean = 0.0;
    int totalEntries = 0;
    std::string currentMonth = "";

    for (size_t date = 0; date < minData.size(); ++date)
    {
        std::string month = minData[date][0].substr(0, 7);
        if (month >= startPeriod)
        {
            break;
        }
        previousMean = std::stod(meanData[date][country]);
    }

    for (size_t date = 0; date < minData.size(); ++date)
    {
        std::string month = minData[date][0].substr(0, 7);

        if (month < startPeriod)
        {
            continue;
        }
        if (month > endPeriod)
        {
            break;
        }

        if (currentMonth.empty())
        {
            currentMonth = month;
        }
        else if (month != currentMonth)
        {
            totalMean /= totalEntries;
            storeToCandleStick(currentMonth, totalMean, totalMax, totalMin, previousMean, country, vectorOfCandlesticks);

            previousMean = totalMean;
            currentMonth = month;
            totalMin = std::numeric_limits<double>::max();
            totalMax = std::numeric_limits<double>::lowest();
            totalMean = 0.0;
            totalEntries = 0;
        }

        calculateStats(minData, maxData, meanData, country, date, totalMin, totalMax, totalMean, totalEntries);
    }

    if (totalEntries > 0)
    {
        totalMean /= totalEntries;
        storeToCandleStick(currentMonth, totalMean, totalMax, totalMin, previousMean, country, vectorOfCandlesticks);
    }
}

// Function to process daily format
void filterCandlestick::processDailyFormat(const std::vector<std::vector<std::string>> &minData,
                                           const std::vector<std::vector<std::string>> &maxData,
                                           const std::vector<std::vector<std::string>> &meanData,
                                           std::vector<candleStick> &vectorOfCandlesticks,
                                           int country,
                                           const std::string &startPeriod,
                                           const std::string &endPeriod)
{
    for (size_t date = 0; date < minData.size(); ++date)
    {
        std::string dateStr = minData[date][0].substr(0, 10);

        if (dateStr < startPeriod)
        {
            continue;
        }
        if (dateStr > endPeriod)
        {
            break;
        }

        try
        {
            double closeValue = (date == 0) ? 0.0 : std::stod(meanData[date - 1][country]);

            candleStick weatherData{
                minData[date][0],
                std::stod(meanData[date][country]),
                std::stod(maxData[date][country]),
                std::stod(minData[date][country]),
                closeValue,
                candleStick::intToCountryType(country)};

            vectorOfCandlesticks.push_back(weatherData);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error processing row " << date << ": " << e.what() << std::endl;
        }
    }
}

std::vector<candleStick> filterCandlestick::filterDataset(std::vector<std::vector<std::vector<std::string>>> datasets)
{
    std::vector<candleStick> vectorOfCandlesticks;

    // Extract datasets
    std::vector<std::vector<std::string>> minData = datasets[0];
    std::vector<std::vector<std::string>> maxData = datasets[1];
    std::vector<std::vector<std::string>> meanData = datasets[2];

    int country = candleStick::stringToCountryInteger(this->country);

    // User-specified range
    std::string startPeriod = this->startDate;
    std::string endPeriod = this->endDate;

    // Process based on format
    processFormat(minData, maxData, meanData, vectorOfCandlesticks, country, startPeriod, endPeriod);

    return vectorOfCandlesticks;
}

void filterCandlestick::printTable(std::vector<candleStick> vectorOfCandlesticks)
{
    for (size_t i = 0; i < vectorOfCandlesticks.size(); ++i)
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
