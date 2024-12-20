#include "filterCandlestick.h"
#include <vector>
#include <iostream>

filterCandlestick::filterCandlestick(std::string _country,
                                     std::string _startDate,
                                     std::string _endDate,
                                     int _startTemp,
                                     int _endTemp)
    : country(_country),
      startDate(_startDate),
      endDate(_endDate),
      startTemp(_startTemp),
      endTemp(_endTemp) {
      };

std::vector<candleStick> filterCandlestick::filterDataset(std::vector<std::vector<std::vector<std::string>>> datasets)
{
    std::vector<candleStick> vectorOfCandlesticks;

    // Extract datasets
    std::vector<std::vector<std::string>> minData = datasets[0];
    std::vector<std::vector<std::string>> maxData = datasets[1];
    std::vector<std::vector<std::string>> meanData = datasets[2];

    int country = candleStick::stringToCountryInteger(this->country);

    // User specifies the range
    std::string startPeriod = this->startDate; // Start of the range (yyyy, yyyy-mm, or yyyy-mm-dd)
    std::string endPeriod = this->endDate;     // End of the range

    // Determine format based on the length of startPeriod and endPeriod
    if (startPeriod.size() == 4 && endPeriod.size() == 4) // yyyy format
    {
        // Aggregate data for the specified range
        double totalMin = std::numeric_limits<double>::max();
        double totalMax = std::numeric_limits<double>::lowest();
        double totalMean = 0.0;
        double previousMean = 0.0;
        int totalEntries = 0;
        std::string currentYear = "";

        for (size_t date = 0; date < minData.size(); ++date)
        {
            // Extract the year from the current row
            std::string year = minData[date][0].substr(0, 4); // Extract yyyy

            // Skip rows outside the specified range
            if (year < startPeriod)
            {
                continue;
            }
            if (year > endPeriod)
            {
                break; // Stop processing after the endPeriod
            }

            if (currentYear.empty())
            {
                currentYear = year;
            }
            else if (year != currentYear)
            {
                // Finalize the previous year's data
                totalMean /= totalEntries;

                try
                {
                    candleStick weatherData{
                        currentYear,
                        totalMean,
                        totalMax,
                        totalMin,
                        previousMean, // For yearly aggregation, open = close = mean
                        candleStick::intToCountryType(country)};

                    vectorOfCandlesticks.push_back(weatherData);
                    previousMean = totalMean;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error processing year " << currentYear << ": " << e.what() << std::endl;
                }

                // Reset for the new year
                currentYear = year;
                totalMin = std::numeric_limits<double>::max();
                totalMax = std::numeric_limits<double>::lowest();
                totalMean = 0.0;
                totalEntries = 0;
            }

            // Aggregate data for the current year
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
    }
    else if (startPeriod.size() == 7 && endPeriod.size() == 7) // yyyy-mm format
    {
        // Aggregate data for the specified range
        double totalMin = std::numeric_limits<double>::max();
        double totalMax = std::numeric_limits<double>::lowest();
        double totalMean = 0.0;
        double previousMean = 0.0;
        int totalEntries = 0;
        std::string currentMonth = "";
        std::string currentTimestamp;

        for (size_t date = 0; date < minData.size(); ++date)
        {
            // Extract the month from the current row
            std::string month = minData[date][0].substr(0, 7); // Extract yyyy-mm

            // Skip rows outside the specified range
            if (month < startPeriod)
            {
                continue;
            }
            if (month > endPeriod)
            {
                break; // Stop processing after the endPeriod
            }

            if (currentMonth.empty())
            {
                currentMonth = month;
            }
            else if (month != currentMonth)
            {
                // Finalize the previous month's data
                totalMean /= totalEntries;

                try
                {
                    candleStick weatherData{
                        currentMonth,
                        totalMean,
                        totalMax,
                        totalMin,
                        previousMean, // For monthly aggregation, open = close = mean
                        candleStick::intToCountryType(country)};

                    vectorOfCandlesticks.push_back(weatherData);
                    previousMean = totalMean;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error processing month " << currentMonth << ": " << e.what() << std::endl;
                }

                // Reset for the new month
                currentMonth = month;
                totalMin = std::numeric_limits<double>::max();
                totalMax = std::numeric_limits<double>::lowest();
                totalMean = 0.0;
                totalEntries = 0;
            }

            // Aggregate data for the current month
            try
            {
                double minValue = std::stod(minData[date][country]);
                double maxValue = std::stod(maxData[date][country]);
                double meanValue = std::stod(meanData[date][country]);

                totalMin = std::min(totalMin, minValue);
                totalMax = std::max(totalMax, maxValue);
                totalMean += meanValue;

                // Store the timestamp for the first entry in the month
                if (totalEntries == 0)
                {
                    currentTimestamp = minData[date][0].substr(0, 10); // Use the first valid date
                }

                totalEntries++;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error processing row " << date << ": " << e.what() << std::endl;
            }
        }
    }
    else if (startPeriod.size() == 10 && endPeriod.size() == 10) // yyyy-mm-dd format
    {
        // Process data directly for yyyy-mm-dd format
        for (size_t date = 0; date < minData.size(); ++date)
        {
            std::string dateStr = minData[date][0].substr(0, 10); // Extract yyyy-mm-dd

            // Skip rows outside the specified range
            if (dateStr < startPeriod)
            {
                continue;
            }
            if (dateStr > endPeriod)
            {
                break; // Stop processing after the endPeriod
            }

            int close_amount = (date == 0) ? 0 : date - 1;

            try
            {
                // Use 0 for the "close" value if date == 0, otherwise fetch from the dataset
                double closeValue = (date == 0) ? 0.0 : std::stod(meanData[close_amount][country]);

                candleStick weatherData{
                    minData[date][0],                   // Timestamp
                    std::stod(meanData[date][country]), // Open
                    std::stod(maxData[date][country]),  // High
                    std::stod(minData[date][country]),  // Low
                    closeValue,                         // Close
                    candleStick::intToCountryType(country)};

                vectorOfCandlesticks.push_back(weatherData);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error processing row " << date << ": " << e.what() << std::endl;
            }
        }
    }

    return vectorOfCandlesticks;
};

void filterCandlestick::printTable(std::vector<candleStick> vectorOfCandlesticks)
{
    // Print the collected weatherDataEntries
    for (auto &entry : vectorOfCandlesticks)
    {
        std::cout << "Country: " << candleStick::countryToString(entry.country) << ", "
                  << "Date: " << entry.timestamp << ", "
                  << "High: " << entry.high << ", "
                  << "Open: " << entry.open << ", "
                  << "Close: " << entry.close << ", "
                  << "Low: " << entry.low << std::endl;
    }
};