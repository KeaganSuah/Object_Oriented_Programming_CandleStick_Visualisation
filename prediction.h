#pragma once

#include "candleStick.h"
#include "filterCandlestick.h"
#include <vector>
#include <string>

///// Class for making future predictions using linear regression. //////
class prediction
{
public:
    // Predict future candlesticks using linear regression
    static void predictLinearRegression(std::vector<candleStick> &vectorOfCandlesticks,
                                        const filterCandlestick &UserFiltered,
                                        unsigned int predictionPeriod);

private:
    // Determine the type of period (years, months, days) from the date format
    static std::pair<std::string, unsigned int> determinePeriodType(const std::string &startDate);

    // Calculate averages for high, low, and close changes
    static void calculatePeriodAverages(const std::vector<candleStick> &candlesticks, const std::string &periodType,
                                        std::vector<double> &avgHighChange, std::vector<double> &avgLowChange,
                                        std::vector<double> &avgCloseChange, std::vector<unsigned int> &countChanges);

    // Generate predictions based on historical averages
    static void generatePredictions(std::vector<candleStick> &vectorOfCandlesticks, unsigned int predictionPeriod,
                                    const std::string &periodType, unsigned int dateIncrement,
                                    const std::vector<double> &avgHighChange, const std::vector<double> &avgLowChange,
                                    const std::vector<double> &avgCloseChange);

    // Increment a date by a given number of days
    static std::string incrementDate(const std::string &date, unsigned int days);
};
