#pragma once

#include <vector>
#include <string>

///// Class to handle reading and processing CSV files. //////
class CSVReader
{
public:
    // Constructor
    CSVReader();

    // Create a 2D vector table from min, max, and mean CSV files
    static std::vector<std::vector<std::vector<std::string>>> create2DVector(const std::string &csvFilename);

    // Tokenize a line from a CSV file
    static std::vector<std::string> tokenise(std::string csvLine, char separator);

private:
    // Check and regenerate missing files
    static void checkAndGenerateFiles(const std::string &csvFilename, const std::vector<std::string> &files);

    // Read a cleaned CSV file into a 2D vector
    static std::vector<std::vector<std::string>> readCleanedCSV(const std::string &filename);

    // Check if a file exists
    static bool checkFileExistence(const std::string &filename);

    // Read the main raw CSV file and generate min, max, and mean files
    static void readRawCSV(const std::string &csvFilename);

    // Write a vector of strings to a CSV file
    static void writeVectorToCSV(const std::string &filename, const std::vector<std::string> &data);

    // Process a single row of data
    static void processRow(const std::vector<std::string> &row,
                           std::vector<double> &minValues,
                           std::vector<double> &maxValues,
                           std::vector<double> &meanValues);

    // Finalize daily data and write to respective files
    static void computeDaysData(const std::string &date,
                                std::vector<double> &minValues,
                                std::vector<double> &maxValues,
                                std::vector<double> &meanValues,
                                int dayRowCount);
};
