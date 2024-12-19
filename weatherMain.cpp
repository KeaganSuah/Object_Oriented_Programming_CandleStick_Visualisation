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

    // Loop through each row of the datasets
    for (size_t date = 0; date < minData.size(); ++date)
    {
        int close_amount;
        if (date == 0)
        {
            close_amount = 0;
        }
        else
        {
            close_amount = date - 1;
        }

        try
        {
            // Create a weatherDataEntry object for the current row
            weatherDataEntry weatherData{
                minData[date][0],
                std::stod(meanData[date][country]),
                std::stod(maxData[date][country]),
                std::stod(minData[date][country]),
                std::stod(meanData[close_amount][country]),
                weatherDataEntry::intToCountryType(country)};

            // Add the object to the weatherVectors
            weatherVectors.push_back(weatherData);
        }
        catch (const std::exception &e)
        {
            // Handle any potential errors during conversion or access
            std::cerr << "Error processing row " << date << ": " << e.what() << std::endl;
        }
    }

    // Print the collected weatherDataEntries (optional)
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
