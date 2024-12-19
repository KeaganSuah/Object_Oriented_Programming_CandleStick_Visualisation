#include "weatherMain.h"
#include "weatherDataEntry.h"
#include "CSVReader.h"
#include <iostream>
#include <vector>

weatherMain::weatherMain()
{
}

void weatherMain::init()
{
    int input;

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void weatherMain::printMenu()
{
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "                    Weather App Menu                  " << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 Enter filter for the weather app
    std::cout << "2: Enter filter for the weather app" << std::endl;
    // 3 Print Statistics Table
    std::cout << "3: Print Statistics Table" << std::endl;
    // 4 Draw Candlesticks
    std::cout << "4: Draw Candlesticks" << std::endl;
    // 5 Make Prediction Range
    std::cout << "5: Make Prediction Range" << std::endl;
    // 6 Exit
    std::cout << "6: Exit" << std::endl;
}

void weatherMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
};

void weatherMain::getFilter()
{
    const std::string csvFilename = "weather_data_EU_1980-2019_temp_only.csv";

    // Handle Min, Max, and Mean CSV files
    std::vector<std::vector<std::vector<std::string>>> datasets = CSVReader::handleMinMaxMeanFiles(csvFilename);

    // Extract datasets
    std::vector<std::vector<std::string>> minData = datasets[0];
    std::vector<std::vector<std::string>> maxData = datasets[1];
    std::vector<std::vector<std::string>> meanData = datasets[2];

    std::vector<weatherDataEntry> weatherVectors;
    int country = 1;

    // User specifies the range
    std::string startPeriod = "1980-06-01"; // Start of the range (yyyy, yyyy-mm, or yyyy-mm-dd)
    std::string endPeriod = "1980-06-30";   // End of the range

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
                    weatherDataEntry weatherData{
                        currentYear,
                        totalMean,
                        totalMax,
                        totalMin,
                        previousMean, // For yearly aggregation, open = close = mean
                        weatherDataEntry::intToCountryType(country)};

                    weatherVectors.push_back(weatherData);
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
                    weatherDataEntry weatherData{
                        currentMonth,
                        totalMean,
                        totalMax,
                        totalMin,
                        previousMean, // For monthly aggregation, open = close = mean
                        weatherDataEntry::intToCountryType(country)};

                    weatherVectors.push_back(weatherData);
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

                weatherDataEntry weatherData{
                    minData[date][0],                   // Timestamp
                    std::stod(meanData[date][country]), // Open
                    std::stod(maxData[date][country]),  // High
                    std::stod(minData[date][country]),  // Low
                    closeValue,                         // Close
                    weatherDataEntry::intToCountryType(country)};

                weatherVectors.push_back(weatherData);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error processing row " << date << ": " << e.what() << std::endl;
            }
        }
    }

    // Print the collected weatherDataEntries
    for (auto &entry : weatherVectors)
    {
        std::cout << "Country: " << weatherDataEntry::countryToString(entry.country) << ", "
                  << "Date: " << entry.timestamp << ", "
                  << "High: " << entry.high << ", "
                  << "Open: " << entry.open << ", "
                  << "Close: " << entry.close << ", "
                  << "Low: " << entry.low << std::endl;
    }
};

void weatherMain::showTableStats() {};
void weatherMain::showCandlestick() {};
void weatherMain::getPrediction() {};

int weatherMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void weatherMain::processUserOption(int userOption)
{
    switch (userOption)
    {
    case 1:
        printHelp();
        break;
    case 2:
        getFilter();
        break;
    case 3:
        showTableStats();
        break;
    case 4:
        showCandlestick();
        break;
    case 5:
        getPrediction();
        break;
    case 6:
        std::cout << "Exiting the Application" << std::endl;
        // Exits the program
        std::exit(0);
    default:
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
        break;
    }
}
