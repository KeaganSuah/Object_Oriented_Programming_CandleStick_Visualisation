#include "weatherMain.h"
#include "CSVReader.h"
#include "filterCandlestick.h"
#include "visualisation.h"
#include "prediction.h"
#include <regex>
#include <iostream>
#include <vector>

// Constructor
///// Code Written by myself to initialise the weatherMain class. //////
weatherMain::weatherMain() {}

///// Code Written with Coursera Assistance to initialise the application and handle the main menu loop. //////
void weatherMain::init()
{
    // Specify the CSV file to process
    const std::string csvFilename = "weather_data_EU_1980-2019_temp_only.csv";

    // Load the datasets into a 3D vector structure
    std::vector<std::vector<std::vector<std::string>>> datasets = CSVReader::create2DVector(csvFilename);

    int input;

    // Continuous loop for menu options
    while (true)
    {
        printMenu();                        // Display the main menu
        input = getUserOption();            // Get user choice
        processUserOption(input, datasets); // Process the chosen option
    }
}

///// Code Written by myself to display the main menu options. //////
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

///// Code Written by myself to display help information. //////
void weatherMain::printHelp()
{
    std::cout << "Help - Analyse the market trends using weather data, apply filters, visualize data, "
              << "and generate predictions to assist decision-making." << std::endl;
}

///// Code Written by myself to prompt the user for filtering parameters and validate input. //////
void weatherMain::getFilter(std::vector<std::vector<std::vector<std::string>>> datasets)
{
    // Prompt the user for filter input
    std::cout << "Enter filter parameters in CSV format (e.g., DE,2016-10,2020-12,-10,35): ";
    std::string userInput;
    std::getline(std::cin, userInput);

    // Tokenize the input into components
    std::vector<std::string> tokens = CSVReader::tokenise(userInput, ',');

    // Validate the input format
    if (tokens.size() != 5)
    {
        std::cerr << "\nInvalid input format. Please provide 5 values: Country, StartDate, EndDate, MinTemp, MaxTemp" << std::endl;
        return;
    }

    // Assign input values to variables
    std::string &country = tokens[0];
    std::string &startDate = tokens[1];
    std::string &endDate = tokens[2];
    std::string &minTemp = tokens[3];
    std::string &maxTemp = tokens[4];

    // Validate date formats using a regex pattern
    const std::regex datePattern(R"(^\d{4}(-\d{2})?(-\d{2})?$)"); // yyyy, yyyy-mm, or yyyy-mm-dd
    if (!std::regex_match(startDate, datePattern) || !std::regex_match(endDate, datePattern))
    {
        std::cerr << "\nInvalid date format. Start and end dates must be in yyyy, yyyy-mm, or yyyy-mm-dd format." << std::endl;
        return;
    }

    // Ensure start and end dates are in the same format
    if (startDate.size() != endDate.size())
    {
        std::cerr << "Inconsistent date format. Start and end dates must have the same format." << std::endl;
        return;
    }

    // Validate the country code
    int countryCode = candleStick::stringToCountryInteger(country);
    if (countryCode == 0)
    {
        std::cerr << "\nInvalid country code: " << country << std::endl;
        return;
    }

    // Validate the temperature range
    try
    {
        int minTemperature = std::stoi(minTemp);
        int maxTemperature = std::stoi(maxTemp);

        if (minTemperature >= maxTemperature)
        {
            std::cerr << "\nInvalid temperature range. Max temperature must be greater than min temperature." << std::endl;
            return;
        }

        // Create a filterCandlestick object with the input values
        filterCandlestick filterObj{
            country,        // Country
            startDate,      // Start date
            endDate,        // End date
            minTemperature, // Minimum temperature
            maxTemperature  // Maximum temperature
        };

        // Apply the filter and store results
        vectorOfCandlesticks = filterObj.filterDataset(datasets);
        UserFiltered = filterObj;

        std::cout << "\nFilter has been set. You can now print the statistics table and candlestick visualization.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error processing input: " << e.what() << std::endl;
    }
}

///// Code Written by myself to display a statistics table of the filtered data. //////
void weatherMain::showTableStats()
{
    if (vectorOfCandlesticks.empty())
    {
        std::cout << "No weather data available to show on statistics table. Please apply a filter first." << std::endl;
    }
    else
    {
        visualisation::printTable(vectorOfCandlesticks, UserFiltered);
    }
}

///// Code Written by myself to visualize the filtered data as candlesticks. //////
void weatherMain::showCandlestick()
{
    if (vectorOfCandlesticks.empty())
    {
        std::cout << "No weather data available to draw candlesticks. Please apply a filter first." << std::endl;
    }
    else
    {
        visualisation vis;
        vis.displayCandleSticks(vectorOfCandlesticks, UserFiltered);
    }
}

///// Code Written by myself to generate predictions for future weather data. //////
void weatherMain::getPrediction()
{
    if (vectorOfCandlesticks.empty())
    {
        std::cout << "Please apply a filter first before generating the prediction." << std::endl;
    }
    else
    {
        int predictionPeriod;
        std::string line;

        std::cout << "Enter the number of prediction periods (integer only): ";
        std::getline(std::cin, line);

        try
        {
            predictionPeriod = std::stoi(line);
            if (predictionPeriod <= 0)
            {
                throw std::invalid_argument("Prediction period must be a positive integer.");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "\nInvalid input. Please enter a positive integer for the prediction period." << std::endl;
            return;
        }

        prediction::predictLinearRegression(vectorOfCandlesticks, UserFiltered, predictionPeriod);
    }
}

///// Code Written with Coursera Assistance to get user input for menu options. //////
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
        std::cerr << "\nInvalid input. Please enter a number between 1 and 6." << std::endl;
    }
    std::cout << std::endl
              << "You chose: " << userOption << std::endl
              << std::endl;
    return userOption;
}

///// Code Written by myself to process the selected menu option. //////
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
        std::cout << "\nInvalid choice. Choose 1-6" << std::endl;
        break;
    }
}
