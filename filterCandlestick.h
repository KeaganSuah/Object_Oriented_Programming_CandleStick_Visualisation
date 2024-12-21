#pragma once

#include <string>
#include <vector>
#include "candleStick.h"

class filterCandlestick
{
public:
    // Constructors
    filterCandlestick(std::string country,
                      std::string startDate,
                      std::string endDate,
                      int startTemp,
                      int endTemp);

    // Default constructor
    filterCandlestick()
        : country(""), startDate(""), endDate(""), startTemp(0), endTemp(0) {}

    // Main filtering function
    std::vector<candleStick> filterDataset(std::vector<std::vector<std::vector<std::string>>> datasets);

    // Print table
    static void printTable(std::vector<candleStick> vectorOfCandlesticks);

    // Member variables
    std::string country;
    std::string startDate;
    std::string endDate;
    int startTemp;
    int endTemp;

private:
    // Helper functions

    // Calculate stats (min, max, mean)
    void calculateStats(const std::vector<std::vector<std::string>> &minData,
                        const std::vector<std::vector<std::string>> &maxData,
                        const std::vector<std::vector<std::string>> &meanData,
                        int country,
                        size_t date,
                        double &totalMin,
                        double &totalMax,
                        double &totalMean,
                        int &totalEntries);

    // Process format
    void processFormat(const std::vector<std::vector<std::string>> &minData,
                       const std::vector<std::vector<std::string>> &maxData,
                       const std::vector<std::vector<std::string>> &meanData,
                       std::vector<candleStick> &vectorOfCandlesticks,
                       int country,
                       const std::string &startPeriod,
                       const std::string &endPeriod);

    // Store data into candleStick object
    void storeToCandleStick(const std::string &period,
                            double totalMean,
                            double totalMax,
                            double totalMin,
                            double previousMean,
                            int country,
                            std::vector<candleStick> &vectorOfCandlesticks);

    // Processing functions
    void processYearlyFormat(const std::vector<std::vector<std::string>> &minData,
                             const std::vector<std::vector<std::string>> &maxData,
                             const std::vector<std::vector<std::string>> &meanData,
                             std::vector<candleStick> &vectorOfCandlesticks,
                             int country,
                             const std::string &startPeriod,
                             const std::string &endPeriod);

    void processMonthlyFormat(const std::vector<std::vector<std::string>> &minData,
                              const std::vector<std::vector<std::string>> &maxData,
                              const std::vector<std::vector<std::string>> &meanData,
                              std::vector<candleStick> &vectorOfCandlesticks,
                              int country,
                              const std::string &startPeriod,
                              const std::string &endPeriod);

    void processDailyFormat(const std::vector<std::vector<std::string>> &minData,
                            const std::vector<std::vector<std::string>> &maxData,
                            const std::vector<std::vector<std::string>> &meanData,
                            std::vector<candleStick> &vectorOfCandlesticks,
                            int country,
                            const std::string &startPeriod,
                            const std::string &endPeriod);
};
