#include "prediction.h"
#include <iostream>

///// Code Written by myself to perform linear regression predictions. //////
void prediction::predictLinearRegression(std::vector<candleStick> &vectorOfCandlesticks,
                                         const filterCandlestick &UserFiltered,
                                         unsigned int predictionPeriod)
{
    // Determine the period type (years, months, or days) and corresponding increment
    std::pair<std::string, unsigned int> periodInfo = determinePeriodType(UserFiltered.startDate);
    std::string periodType = periodInfo.first;
    unsigned int dateIncrement = periodInfo.second;

    // Ensure there is historical data available
    if (vectorOfCandlesticks.empty())
    {
        std::cerr << "No historical data available for prediction." << std::endl;
        return;
    }

    // Initialize average change and count vectors for high, low, and close prices
    std::vector<double> avgHighChange(12, 0.0), avgLowChange(12, 0.0), avgCloseChange(12, 0.0);
    std::vector<unsigned int> countChanges(12, 0);

    // Calculate period-wise averages from historical data
    calculatePeriodAverages(vectorOfCandlesticks, periodType, avgHighChange, avgLowChange, avgCloseChange, countChanges);

    // Generate future predictions based on averages
    generatePredictions(vectorOfCandlesticks, predictionPeriod, periodType, dateIncrement, avgHighChange, avgLowChange, avgCloseChange);

    std::cout << "Predictions completed for " << predictionPeriod << " " << periodType << "." << std::endl;
}

///// Code Written by myself to determine the period type and increment based on the date format. //////
std::pair<std::string, unsigned int> prediction::determinePeriodType(const std::string &startDate)
{
    // Check the length of the start date to determine the period type
    if (startDate.size() == 4) // yyyy format
        return std::make_pair("years", 365);
    if (startDate.size() == 7) // yyyy-mm format
        return std::make_pair("months", 30);
    if (startDate.size() == 10) // yyyy-mm-dd format
        return std::make_pair("days", 1);

    // Log an error if the date format is invalid
    std::cerr << "Error: Invalid date format." << std::endl;
    return std::make_pair("", 0); // Return an invalid result
}

///// Code Written by myself to calculate averages for high, low, and close prices. //////
void prediction::calculatePeriodAverages(const std::vector<candleStick> &candlesticks, const std::string &periodType,
                                         std::vector<double> &avgHighChange, std::vector<double> &avgLowChange,
                                         std::vector<double> &avgCloseChange, std::vector<unsigned int> &countChanges)
{
    // Loop through candlesticks to calculate differences
    for (unsigned int i = 1; i < candlesticks.size(); ++i)
    {
        // Extract periods based on the period type
        std::string prevDate = candlesticks[i - 1].timestamp.substr(0, periodType == "years" ? 4 : (periodType == "months" ? 7 : 10));
        std::string currDate = candlesticks[i].timestamp.substr(0, periodType == "years" ? 4 : (periodType == "months" ? 7 : 10));

        // Calculate changes only when the period changes
        if (prevDate != currDate)
        {
            int periodIndex = (periodType == "months")
                                  ? std::stoi(prevDate.substr(5, 2)) - 1
                              : (periodType == "years")
                                  ? std::stoi(prevDate.substr(0, 4)) % 12
                                  : 0;

            // Update average changes and count
            avgHighChange[periodIndex] += (candlesticks[i].high - candlesticks[i - 1].high);
            avgLowChange[periodIndex] += (candlesticks[i].low - candlesticks[i - 1].low);
            avgCloseChange[periodIndex] += (candlesticks[i].close - candlesticks[i - 1].close);
            countChanges[periodIndex]++;
        }
    }

    // Finalize averages by dividing by counts
    for (unsigned int i = 0; i < 12; ++i)
    {
        if (countChanges[i] > 0)
        {
            avgHighChange[i] /= countChanges[i];
            avgLowChange[i] /= countChanges[i];
            avgCloseChange[i] /= countChanges[i];
        }
    }
}

///// Code Written by myself to generate predictions for the future. //////
void prediction::generatePredictions(std::vector<candleStick> &vectorOfCandlesticks, unsigned int predictionPeriod,
                                     const std::string &periodType, unsigned int dateIncrement,
                                     const std::vector<double> &avgHighChange, const std::vector<double> &avgLowChange,
                                     const std::vector<double> &avgCloseChange)
{
    // Get the last recorded timestamp and price data
    std::string lastTimestamp = vectorOfCandlesticks.back().timestamp;
    double previousClose = vectorOfCandlesticks.back().close;
    double previousHigh = vectorOfCandlesticks.back().high;
    double previousLow = vectorOfCandlesticks.back().low;

    // Generate predictions for the specified period
    for (unsigned int i = 1; i <= predictionPeriod; ++i)
    {
        int periodIndex = (periodType == "months")
                              ? (std::stoi(lastTimestamp.substr(5, 2)) - 1 + i) % 12
                          : (periodType == "years")
                              ? (std::stoi(lastTimestamp.substr(0, 4)) + i) % 12
                              : 0;

        // Predict high, low, and open prices based on averages
        double predictedHigh = previousHigh + avgHighChange[periodIndex];
        double predictedLow = previousLow + avgLowChange[periodIndex];
        double predictedOpen = previousClose + avgCloseChange[periodIndex];

        // Increment the date by the calculated step
        std::string predictedTimestamp = incrementDate(lastTimestamp, i * dateIncrement);

        // Create a new candleStick object for the prediction
        candleStick predictedCandle(predictedTimestamp, predictedOpen, predictedHigh, predictedLow, previousClose, vectorOfCandlesticks.back().country);

        // Add the prediction to the vector
        vectorOfCandlesticks.push_back(predictedCandle);

        // Update previous values for the next prediction
        previousClose = predictedOpen;
        previousHigh = predictedHigh;
        previousLow = predictedLow;
    }
}

///// Code Written by myself to increment a date by a specified number of days. //////
std::string prediction::incrementDate(const std::string &date, unsigned int days)
{
    // Parse the year, month, and day from the date string
    int year = std::stoi(date.substr(0, 4));
    int month = (date.size() >= 7) ? std::stoi(date.substr(5, 2)) : 1;
    int day = (date.size() == 10) ? std::stoi(date.substr(8, 2)) : 1;

    // Increment the day and adjust month/year if necessary
    day += days;
    while (day > 30)
    {
        day -= 30;
        month++;
    }
    while (month > 12)
    {
        month -= 12;
        year++;
    }

    // Build the new date string
    std::string newDate = std::to_string(year);
    if (date.size() >= 7)
    {
        newDate += "-";
        newDate += (month < 10 ? "0" : "") + std::to_string(month);
    }
    if (date.size() == 10)
    {
        newDate += "-";
        newDate += (day < 10 ? "0" : "") + std::to_string(day);
    }
    return newDate;
}
