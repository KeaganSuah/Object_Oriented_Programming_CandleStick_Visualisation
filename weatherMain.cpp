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
    int countryCodeStr = 2;
    int date = 1;
    std::vector<weatherDataEntry> weatherVectors;

    const std::string csvFilename = "weather_data_EU_1980-2019_temp_only.csv";

    // Handle Min, Max, and Mean CSV files
    auto datasets = CSVReader::handleMinMaxMeanFiles(csvFilename);

    // Extract datasets
    auto minData = datasets[0];
    auto maxData = datasets[1];
    auto meanData = datasets[2];

    weatherDataEntry weatherData{
        minData[date][0],                          // Keep the timestamp as is
        std::stod(minData[date][countryCodeStr]),  // Convert to double
        std::stod(minData[date][countryCodeStr]),  // Convert to double
        std::stod(maxData[date][countryCodeStr]),  // Convert to double
        std::stod(meanData[date][countryCodeStr]), // Convert to double
        weatherDataEntry::intToCountryType(countryCodeStr)};

    weatherVectors.push_back(weatherData);

    for (weatherDataEntry &value : weatherVectors)
    {
        // Print the timestamp
        std::cout << "timestamp: " << value.timestamp << std::endl;
        std::cout << ":open " << value.open << std::endl;
        std::cout << ":close " << value.close << std::endl;
        std::cout << "high: " << value.high << std::endl;
        std::cout << "low: " << value.low << std::endl;
        std::cout << "Timestamp: " << weatherDataEntry::countryToString(value.country) << std::endl;
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
