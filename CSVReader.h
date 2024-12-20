#pragma once

#include "candleStick.h"
#include <vector>
#include <string>

class CSVReader
{
public:
    CSVReader();
    static std::vector<std::string> tokenise(std::string csvLine, char separator);
    // Function to handle min, max, mean CSV files
    // Reads them if they exist; creates them using readCSV if they don't exist
    static std::vector<std::vector<std::vector<std::string>>> handleMinMaxMeanFiles(const std::string &csvFilename);

private:
    static void readCSV(const std::string &csvFilename);
    static void writeVectorToCSV(const std::string &filename, const std::vector<std::string> &data);

    // Function to read an existing CSV file into a vector of vectors
    static std::vector<std::vector<std::string>> readExistingCSV(const std::string &filename);
    // Function to check if a file exists
    static bool fileExists(const std::string &filename);
};