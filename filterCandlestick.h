#pragma once

#include <string>
#include <vector>
#include "candleStick.h"

///// Class to filter and process candlestick data based on user-defined criteria. //////
class filterCandlestick
{
public:
    // Constructor to initialize the filter with user-specified criteria
    filterCandlestick(std::string country,
                      std::string startDate,
                      std::string endDate,
                      int startTemp,
                      int endTemp);

    // Default constructor
    filterCandlestick()
        : country(""), startDate(""), endDate(""), startTemp(0), endTemp(0) {}

    // Main function to filter the dataset
    std::vector<candleStick> filterDataset(std::vector<std::vector<std::vector<std::string>>> datasets);

    // Member variables to store filter parameters
    std::string country;
    std::string startDate;
    std::string endDate;
    int startTemp;
    int endTemp;

private:
    // Helper function to calculate stats (min, max, mean)
    void calculateStats(const std::vector<std::vector<std::string>> &minData,
                        const std::vector<std::vector<std::string>> &maxData,
                        const std::vector<std::vector<std::string>> &meanData,
                        int country,
                        unsigned int date,
                        double &totalMin,
                        double &totalMax,
                        double &totalMean,
                        int &totalEntries);

    // Generalized function to process data based on granularity (year, month, day)
    void processData(const std::vector<std::vector<std::string>> &minData,
                     const std::vector<std::vector<std::string>> &maxData,
                     const std::vector<std::vector<std::string>> &meanData,
                     std::vector<candleStick> &vectorOfCandlesticks,
                     int country,
                     const std::string &startPeriod,
                     const std::string &endPeriod,
                     const std::string &granularity);

    // Helper function to determine granularity and process data accordingly
    void processFormat(const std::vector<std::vector<std::string>> &minData,
                       const std::vector<std::vector<std::string>> &maxData,
                       const std::vector<std::vector<std::string>> &meanData,
                       std::vector<candleStick> &vectorOfCandlesticks,
                       int country,
                       const std::string &startPeriod,
                       const std::string &endPeriod);

    // Helper function to store data into a candleStick object
    void storeToCandleStick(const std::string &period,
                            double totalMean,
                            double totalMax,
                            double totalMin,
                            double previousMean,
                            int country,
                            std::vector<candleStick> &vectorOfCandlesticks);
};
