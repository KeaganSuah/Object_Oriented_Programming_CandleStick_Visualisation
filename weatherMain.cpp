#include "weatherMain.h"
#include "CSVReader.h"
#include "filterCandlestick.h"
#include "visualisation.h"
#include "prediction.h"
#include <regex>
#include <iostream>
#include <vector>

// Constructor
///// Code Written by myself to initialize the weatherMain class. //////
weatherMain::weatherMain() {}

///// Code Written with Coursera Assistance to initialize the application and handle the main menu loop. //////
void weatherMain::init()
{
    // Specify the CSV file to process
    const std::string csvFilename = "weather_data_EU_1980-2019_temp_only.csv";

    // Load datasets from the CSV file into a 3D vector
    std::vector<std::vector<std::vector<std::string>>> datasets = CSVReader::create2DVector(csvFilename);

    int input;

    // Loop to handle menu options until the user exits
    while (true)
    {
        printMenu();                        // Display the menu
        input = getUserOption();            // Get user's choice
        processUserOption(input, datasets); // Perform the selected action
    }
}

///// Code Written by myself to display the main menu options. //////
void weatherMain::printMenu()
{
    // Print the available options for the user
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
    // Display a help message for the user
    std::cout << "Help - Analyse weather trends with historical data, apply custom filters, visualise patterns, "
              << "and predict future conditions to gain valuable insights." << std::endl;
}

///// Code Written by myself to prompt the user for filtering parameters and validate input. //////
void weatherMain::getFilter(std::vector<std::vector<std::vector<std::string>>> datasets)
{
    // Prompt the user for filter parameters
    std::cout << "Select a country from (AT, BE, BG, CH, CZ, DE, DK, EE, ES, FI, FR, GB, GR, HR, HU, IE, IT, LT, LU, LV, NL, NO, PL, PT, RO, SE, SI, SK)." << std::endl;
    std::cout << "Select starting period. Choose from following formats(YYYY-MM-DD, YYYY-MM, YYYY)" << std::endl;
    std::cout << "Select closing period. Choose from following formats(YYYY-MM-DD, YYYY-MM, YYYY)" << std::endl;
    std::cout << "Select lowest temperature. Enter in integer format" << std::endl;
    std::cout << "Select highest temperature. Enter in integer format" << std::endl;
    std::cout << std::endl;
    std::cout << "The final format of the input: country, filter for time, starting period, ending period, lowest temperature, highest temperature" << std::endl;
    std::cout << "Enter filter parameters in CSV format (e.g., DE,2016-10,2020-12,-10,35): ";
    std::string userInput;
    std::getline(std::cin, userInput);

    // Split the input into components
    std::vector<std::string> tokens = CSVReader::tokenise(userInput, ',');

    // Check that the correct number of parameters is provided
    if (tokens.size() != 5)
    {
        std::cerr << "\nInvalid input format. Please provide 5 values: Country, StartDate, EndDate, MinTemp, MaxTemp" << std::endl;
        return;
    }

    // Extract individual filter parameters
    std::string &country = tokens[0];
    std::string &startDate = tokens[1];
    std::string &endDate = tokens[2];
    std::string &minTemp = tokens[3];
    std::string &maxTemp = tokens[4];

    // Validate the date format using regex
    const std::regex datePattern(R"(^\d{4}(-\d{2})?(-\d{2})?$)"); // yyyy, yyyy-mm, or yyyy-mm-dd
    if (!std::regex_match(startDate, datePattern) || !std::regex_match(endDate, datePattern))
    {
        std::cerr << "\nInvalid date format. Start and end dates must be in yyyy, yyyy-mm, or yyyy-mm-dd format." << std::endl;
        return;
    }

    // Ensure start and end dates have the same format
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

        // Ensure minTemp is less than maxTemp
        if (minTemperature >= maxTemperature)
        {
            std::cerr << "\nInvalid temperature range. Max temperature must be greater than min temperature." << std::endl;
            return;
        }

        // Create a filterCandlestick object with the specified parameters
        filterCandlestick filterObj{
            country,        // Country
            startDate,      // Start date
            endDate,        // End date
            minTemperature, // Minimum temperature
            maxTemperature  // Maximum temperature
        };

        // Filter the datasets and store the result
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
        // Notify the user if no data is available
        std::cout << "No weather data available to show on statistics table. Please apply a filter first." << std::endl;
    }
    else
    {
        // Display the statistics table
        visualisation::printTable(vectorOfCandlesticks, UserFiltered);
    }
}

///// Code Written by myself to visualize the filtered data as candlesticks. //////
void weatherMain::showCandlestick()
{
    if (vectorOfCandlesticks.empty())
    {
        // Notify the user if no data is available
        std::cout << "No weather data available to draw candlesticks. Please apply a filter first." << std::endl;
    }
    else
    {
        // Display the candlestick visualization
        visualisation vis;
        vis.displayCandleSticks(vectorOfCandlesticks, UserFiltered);
    }
}

///// Code Written by myself to generate predictions for future weather data. //////
void weatherMain::getPrediction()
{
    if (vectorOfCandlesticks.empty())
    {
        // Notify the user if no data is available
        std::cout << "Please apply a filter first before generating the prediction." << std::endl;
    }
    else
    {
        int predictionPeriod;
        std::string line;

        // Prompt the user to enter the number of prediction periods
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

        // Generate predictions using the prediction class
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
        // Parse the user's input as an integer
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
        // Display help information
        printHelp();
        break;
    case 2:
        // Set a filter for the data
        getFilter(datasets);
        break;
    case 3:
        // Display statistics table
        showTableStats();
        break;
    case 4:
        // Visualize data as candlesticks
        showCandlestick();
        break;
    case 5:
        // Generate predictions
        getPrediction();
        break;
    case 6:
        std::cout << "Exiting the Application" << std::endl;
        std::exit(0); // Exit the application
    default:
        std::cout << "\nInvalid choice. Choose 1-6" << std::endl;
        break;
    }
}
