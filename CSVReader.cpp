#include "CSVReader.h"
#include <iostream>
#include <fstream>

// Constructor
///// Code Written by myself to initialize the CSVReader object. //////
CSVReader::CSVReader() {}

///// Code Written by myself to read and process cleaned min, max, and mean files into a 2D vector table. //////
std::vector<std::vector<std::vector<std::string>>> CSVReader::create2DVector(const std::string &csvFilename)
{
    const std::string minFile = "min.csv";
    const std::string maxFile = "max.csv";
    const std::string meanFile = "mean.csv";

    // Ensure the required files exist or regenerate them
    checkFilesRequired(csvFilename, std::vector<std::string>{minFile, maxFile, meanFile});

    // Load data from the cleaned files and return as a 2D vector
    return {
        readCleanedCSV(minFile),
        readCleanedCSV(maxFile),
        readCleanedCSV(meanFile)};
}

///// Code Written by myself to check and generate missing files if required. //////
void CSVReader::checkFilesRequired(const std::string &csvFilename, const std::vector<std::string> &files)
{
    bool missingFiles = false;

    // Check if any required files are missing
    for (const std::string &file : files)
    {
        if (!checkFileExist(file))
        {
            missingFiles = true;
            break;
        }
    }

    if (missingFiles)
    {
        std::cout << "One or more files are missing. Regenerating files..." << std::endl;

        // Remove existing files
        for (const std::string &file : files)
        {
            if (checkFileExist(file))
                std::remove(file.c_str());
        }

        // Generate files from the raw CSV data
        readRawCSV(csvFilename);
    }
}

///// Code Written by Coursera to read an existing CSV file into a 2D vector. //////
std::vector<std::vector<std::string>> CSVReader::readCleanedCSV(const std::string &filename)
{
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    std::string line;

    // Check if the file can be opened
    if (!file.is_open())
    {
        throw std::runtime_error("File not found: " + filename);
    }

    // Read each line from the file and tokenize it into a vector
    while (std::getline(file, line))
    {
        data.push_back(tokenise(line, ','));
    }

    file.close();
    return data;
}

///// Code Written by myself to check if a file exists. //////
bool CSVReader::checkFileExist(const std::string &filename)
{
    std::ifstream file(filename);
    return file.is_open();
}

///// Code Written with Coursera assistance to read raw CSV data and generate min, max, and mean files. //////
void CSVReader::readRawCSV(const std::string &csvFilename)
{
    std::ifstream csvFile(csvFilename);
    std::string line;

    // Check if the raw CSV file can be opened
    if (!csvFile.is_open())
    {
        std::cerr << "Error: Unable to open file: " << csvFilename << std::endl;
        return;
    }

    // Initialize variables for processing weather data
    std::vector<double> minValues, maxValues, meanValues;
    std::string currentDate;
    int dayRowCount = 0;

    // Process the CSV file line by line
    while (std::getline(csvFile, line))
    {
        // Skip header row
        if (line.find("utc_timestamp") != std::string::npos)
            continue;

        std::vector<std::string> weatherLine = tokenise(line, ',');
        if (weatherLine.empty())
        {
            std::cerr << "Error: Empty or malformed line." << std::endl;
            continue;
        }

        // Extract date from the timestamp
        std::string date = weatherLine[0].substr(0, 10);

        // Check if the date has changed and finalize the previous day's data
        if (currentDate.empty())
        {
            currentDate = date;
        }
        else if (date != currentDate)
        {
            computeDaysData(currentDate, minValues, maxValues, meanValues, dayRowCount);

            // Reset variables for the next day's data
            minValues.clear();
            maxValues.clear();
            meanValues.clear();
            dayRowCount = 0;
            currentDate = date;
        }

        // Process the current row
        processRow(weatherLine, minValues, maxValues, meanValues);
        dayRowCount++;
    }

    // Finalize the last day's data
    if (dayRowCount > 0)
    {
        computeDaysData(currentDate, minValues, maxValues, meanValues, dayRowCount);
    }

    std::cout << "The required files have been created successfully." << std::endl;
}

///// Code Written by myself to write a vector of strings to a CSV file. //////
void CSVReader::writeVectorToCSV(const std::string &filename, const std::vector<std::string> &data)
{
    std::ofstream file(filename, std::ios::app);

    // Check if the file can be opened for writing
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    // Write each value in the vector to the file, separated by commas
    for (unsigned int i = 0; i < data.size(); ++i)
    {
        file << data[i];
        if (i < data.size() - 1)
            file << ",";
    }

    file << "\n"; // Add a newline at the end
    file.close();
}

///// Code Written by Coursera to tokenize a CSV line. //////
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    std::size_t start = 0, end;

    // Extract tokens by finding the separator
    while ((end = csvLine.find(separator, start)) != std::string::npos)
    {
        if (start < end)
        {
            tokens.push_back(csvLine.substr(start, end - start));
        }
        start = end + 1;
    }

    // Add the last token
    if (start < csvLine.length())
    {
        tokens.push_back(csvLine.substr(start));
    }

    return tokens;
}

///// Code Written by myself to process a single row of weather data. //////
void CSVReader::processRow(const std::vector<std::string> &row,
                           std::vector<double> &minValues,
                           std::vector<double> &maxValues,
                           std::vector<double> &meanValues)
{
    std::vector<std::string> subset(row.begin() + 1, row.end());

    // Initialize vectors if empty
    if (minValues.empty())
    {
        minValues.resize(subset.size(), 1e9);
        maxValues.resize(subset.size(), -1e9);
        meanValues.resize(subset.size(), 0.0);
    }

    // Update min, max, and mean values
    for (unsigned int i = 0; i < subset.size(); ++i)
    {
        double value = std::stod(subset[i]);
        minValues[i] = std::min(minValues[i], value);
        maxValues[i] = std::max(maxValues[i], value);
        meanValues[i] += value;
    }
}

///// Code Written by myself to compute daily weather data statistics. //////
void CSVReader::computeDaysData(const std::string &date,
                                std::vector<double> &minValues,
                                std::vector<double> &maxValues,
                                std::vector<double> &meanValues,
                                int dayRowCount)
{
    // Calculate mean values for each parameter
    for (unsigned int i = 0; i < meanValues.size(); ++i)
    {
        meanValues[i] /= dayRowCount;
    }

    // Convert numerical data to strings
    std::vector<std::string> minStr, maxStr, meanStr;
    for (unsigned int i = 0; i < meanValues.size(); ++i)
    {
        minStr.push_back(std::to_string(minValues[i]));
        maxStr.push_back(std::to_string(maxValues[i]));
        meanStr.push_back(std::to_string(meanValues[i]));
    }

    // Add the date as the first value
    minStr.insert(minStr.begin(), date);
    maxStr.insert(maxStr.begin(), date);
    meanStr.insert(meanStr.begin(), date);

    // Write data to corresponding files
    writeVectorToCSV("min.csv", minStr);
    writeVectorToCSV("max.csv", maxStr);
    writeVectorToCSV("mean.csv", meanStr);
}
