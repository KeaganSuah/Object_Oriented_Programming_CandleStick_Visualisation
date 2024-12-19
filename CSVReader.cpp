#include <cstdio> // For std::remove
#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{
}

std::vector<std::vector<std::vector<std::string>>> CSVReader::handleMinMaxMeanFiles(const std::string &csvFilename)
{
    const std::string minFile = "min.csv";
    const std::string maxFile = "max.csv";
    const std::string meanFile = "mean.csv";

    std::vector<std::vector<std::string>> minData, maxData, meanData;

    // Check if any of the files does not exist
    if (!CSVReader::fileExists(minFile) || !CSVReader::fileExists(maxFile) || !CSVReader::fileExists(meanFile))
    {
        std::cout << "One or more files are missing. Deleting existing files and regenerating using readCSV..." << std::endl;

        // Delete the files if they exist
        if (CSVReader::fileExists(minFile))
            std::remove(minFile.c_str());
        if (CSVReader::fileExists(maxFile))
            std::remove(maxFile.c_str());
        if (CSVReader::fileExists(meanFile))
            std::remove(meanFile.c_str());

        // Generate all files
        CSVReader::readCSV(csvFilename);

        // Reload the newly created files
        minData = CSVReader::readExistingCSV(minFile);
        maxData = CSVReader::readExistingCSV(maxFile);
        meanData = CSVReader::readExistingCSV(meanFile);
    }
    else
    {
        // All files exist, read them
        std::cout << "All files exist. Reading data from existing files..." << std::endl;
        minData = CSVReader::readExistingCSV(minFile);
        maxData = CSVReader::readExistingCSV(maxFile);
        meanData = CSVReader::readExistingCSV(meanFile);
    }

    // Return all three datasets
    return {minData, maxData, meanData};
}

std::vector<std::vector<std::string>> CSVReader::readExistingCSV(const std::string &filename)
{
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        throw std::runtime_error("File not found: " + filename);
    }

    while (std::getline(file, line))
    {
        std::vector<std::string> row = CSVReader::tokenise(line, ',');
        data.push_back(row);
    }

    file.close();
    return data;
}

bool CSVReader::fileExists(const std::string &filename)
{
    std::ifstream file(filename);
    return file.is_open();
}

void CSVReader::readCSV(const std::string &csvFilename)
{
    std::ifstream csvFile{csvFilename};
    std::string line;

    if (csvFile.is_open())
    {
        std::vector<double> min, max, mean;
        std::string currentDate;
        int dayRowCount = 0; // Tracks rows for the current day

        while (std::getline(csvFile, line))
        {
            // Ignore the header
            if (currentDate.empty() && line.find("utc_timestamp") != std::string::npos)
            {
                continue; // Skip the header
            }

            std::vector<std::string> weatherLine = tokenise(line, ',');
            if (weatherLine.empty())
            {
                std::cerr << "Error: Empty or malformed line." << std::endl;
                continue;
            }

            // Extract the date part
            std::string date = weatherLine[0].substr(0, 10);

            // If the day changes, finalize and reset vectors
            if (currentDate.empty())
            {
                currentDate = date;
            }
            else if (date != currentDate)
            {
                if (dayRowCount > 0) // Ensure we have valid data for the day
                {
                    for (double &value : mean)
                    {
                        value /= dayRowCount; // Calculate the mean for the day
                    }
                }

                // Convert vectors to strings and add the date
                std::vector<std::string> minStr, maxStr, meanStr;
                for (size_t i = 0; i < mean.size(); ++i)
                {
                    minStr.push_back(std::to_string(min[i]));
                    maxStr.push_back(std::to_string(max[i]));
                    meanStr.push_back(std::to_string(mean[i]));
                }

                minStr.insert(minStr.begin(), currentDate);
                maxStr.insert(maxStr.begin(), currentDate);
                meanStr.insert(meanStr.begin(), currentDate);

                // Append data to CSV files
                writeVectorToCSV("min.csv", minStr);
                writeVectorToCSV("max.csv", maxStr);
                writeVectorToCSV("mean.csv", meanStr);

                // Reset vectors and counters for the new day
                min.clear();
                max.clear();
                mean.clear();
                dayRowCount = 0;
                currentDate = date;
            }

            // Process the current day's data (as before)
            std::vector<std::string> subset(weatherLine.begin() + 1, weatherLine.end());
            if (subset.empty())
            {
                std::cerr << "Error: No numeric values found for line: " << line << std::endl;
                continue;
            }

            if (dayRowCount == 0)
            {
                min.resize(subset.size(), std::numeric_limits<double>::max());
                max.resize(subset.size(), std::numeric_limits<double>::lowest());
                mean.resize(subset.size(), 0.0);
            }

            for (size_t i = 0; i < subset.size(); ++i)
            {
                double value = std::stod(subset[i]);
                if (value < min[i])
                    min[i] = value;
                if (value > max[i])
                    max[i] = value;
                mean[i] += value;
            }

            dayRowCount++;
        }

        // Finalize the last day's data (as before)
        if (dayRowCount > 0)
        {
            for (double &value : mean)
            {
                value /= dayRowCount; // Calculate the mean for the last day
            }

            std::vector<std::string> minStr, maxStr, meanStr;
            for (size_t i = 0; i < mean.size(); ++i)
            {
                minStr.push_back(std::to_string(min[i]));
                maxStr.push_back(std::to_string(max[i]));
                meanStr.push_back(std::to_string(mean[i]));
            }

            minStr.insert(minStr.begin(), currentDate);
            maxStr.insert(maxStr.begin(), currentDate);
            meanStr.insert(meanStr.begin(), currentDate);

            writeVectorToCSV("min.csv", minStr);
            writeVectorToCSV("max.csv", maxStr);
            writeVectorToCSV("mean.csv", meanStr);
        }

        std::cout << "The required files have been created successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error: Unable to open file: " << csvFilename << std::endl;
    }
}


void CSVReader::writeVectorToCSV(const std::string &filename, const std::vector<std::string> &data)
{
    // Open the file in append mode
    std::ofstream file(filename, std::ios::app);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    // Write the vector as a single row
    for (size_t i = 0; i < data.size(); ++i)
    {
        file << data[i];
        if (i < data.size() - 1)
        {
            file << ","; // Add a comma between elements
        }
    }

    file << "\n"; // End the row with a newline
    file.close();
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do
    {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end)
            break;
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        else
            token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}
