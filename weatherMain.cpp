#include "weatherMain.h"
#include "CSVReader.h"
#include "filterCandlestick.h"
#include <iostream>
#include <vector>

weatherMain::weatherMain() {}

void weatherMain::init()
{
    // Get CSV and create
    const std::string csvFilename = "weather_data_EU_1980-2019_temp_only.csv";
    std::vector<std::vector<std::vector<std::string>>> datasets = CSVReader::handleMinMaxMeanFiles(csvFilename);

    int input;
    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input, datasets);
    }
}

void weatherMain::printMenu()
{
    std::cout << std::endl
              << "------------------------------------------------------" << std::endl;
    std::cout << "                    Weather App Menu                  " << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "1: Print help " << std::endl;
    std::cout << "2: Enter filter for the weather app" << std::endl;
    std::cout << "3: Print Statistics Table" << std::endl;
    std::cout << "4: Draw Candlesticks" << std::endl;
    std::cout << "5: Make Prediction Range" << std::endl;
    std::cout << "6: Exit" << std::endl;
}

void weatherMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void weatherMain::getFilter(std::vector<std::vector<std::vector<std::string>>> datasets)
{
    // Prompt user for input
    std::cout << "Enter filter parameters in CSV format (e.g., DE,2016-10,2020-12,23,-18): ";
    std::string userInput;
    std::getline(std::cin, userInput);

    // Tokenize the input
    std::vector<std::string> tokens = CSVReader::tokenise(userInput, ',');

    // Validate input
    if (tokens.size() != 5)
    {
        std::cerr << "Invalid input format. Please provide 5 values: Country, StartDate, EndDate, MinTemp, MaxTemp" << std::endl;
        return;
    }

    // Validate date formats
    const std::string &startDate = tokens[1];
    const std::string &endDate = tokens[2];

    if (!(startDate.size() == 4 || startDate.size() == 7 || startDate.size() == 10) ||
        !(endDate.size() == 4 || endDate.size() == 7 || endDate.size() == 10))
    {
        std::cerr << "Invalid date format. Start and end dates must be in yyyy, yyyy-mm, or yyyy-mm-dd format." << std::endl;
        return;
    }
    else if (startDate.size() != endDate.size())
    {
        std::cerr << "Inconsistant date format. Start and end dates must be the same format." << std::endl;
        return;
    }

    try
    {
        // Convert country string to integer
        int countryCode = candleStick::stringToCountryInteger(tokens[0]);
        if (countryCode == 0)
        {
            std::cerr << "Invalid country code: " << tokens[0] << std::endl;
            return;
        }

        if (tokens[3] <= tokens[4])
        {
            std::cerr << "Max Tempature has to be higher than Min Tempature: " << tokens[3] << "is" << tokens[4] << std::endl;
            return;
        }

        // Create the filterCandlestick object
        filterCandlestick filterObj{
            tokens[0],            // Country (keep as string for filtering logic)
            startDate,            // Start date
            endDate,              // End date
            std::stoi(tokens[3]), // Minimum temperature
            std::stoi(tokens[4])  // Maximum temperature
        };

        // Filter the dataset
        vectorOfCandlesticks = filterObj.filterDataset(datasets);
        UserFiltered = filterObj;
        std::cout << std::endl
                  << "Filter has been set. You can now print the statistic table and candlestick visualization." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error processing input: " << e.what() << std::endl;
    }
}

void weatherMain::showTableStats()
{
    if (vectorOfCandlesticks.empty())
    {
        std::cout << "No Weather data available to show on statistics table. Please apply a filter first." << std::endl;
        return;
    }
    else
    {
        filterCandlestick::printTable(vectorOfCandlesticks);
    }
}

void weatherMain::showCandlestick()
{
    if (vectorOfCandlesticks.empty())
    {
        std::cout << "No weather data available to draw candlestick. Please apply a filter first." << std::endl;
        return;
    }
    else
    {
        int startTemp = UserFiltered.startTemp;
        int endTemp = UserFiltered.endTemp;
        while (startTemp >= endTemp)
        {
            int blankCount = 0;
            while (blankCount < (4 - std::to_string(startTemp).size()))
            {
                std::cout << " ";
                ++blankCount;
            }
            std::cout << startTemp << ":  ";
            for (auto &candleStick : vectorOfCandlesticks)
            {
                double minVal = (candleStick.open < candleStick.close) ? candleStick.open : candleStick.close; // Minimum
                double maxVal = (candleStick.open > candleStick.close) ? candleStick.open : candleStick.close; // Maximum

                if (startTemp <= candleStick.high & startTemp > maxVal)
                {
                    std::cout << "| ";
                }
                else if (startTemp <= maxVal & startTemp > minVal)
                {
                    std::cout << "█ ";
                }
                else if (startTemp < minVal & startTemp > candleStick.low)
                {
                    std::cout << "| ";
                }
                else
                {
                    std::cout << "· ";
                }
            }
            std::cout << std::endl;
            --startTemp;
        }
    }
}

void weatherMain::getPrediction()
{
    if (vectorOfCandlesticks.empty())
    {
        std::cout << "Please apply a filter first before generating the prediction." << std::endl;
        return;
    }
    else
    {
        std::cout << "Prediction feature is not implemented yet." << std::endl;
    }
}

int weatherMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << std::endl
              << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Invalid input. Please enter a number between 1 and 6." << std::endl;
    }
    std::cout << std::endl
              << "You chose: " << userOption << std::endl
              << std::endl;
    return userOption;
}

void weatherMain::processUserOption(int userOption, std::vector<std::vector<std::vector<std::string>>> datasets)
{
    switch (userOption)
    {
    case 1:
        printHelp();
        break;
    case 2:
        getFilter(datasets);
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
        std::exit(0);
    default:
        std::cout << "Invalid choice. Choose 1-6" << std::endl;
        break;
    }
}
