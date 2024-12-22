#include "candleStick.h"
#include <map>

// Constructor Implementation
///// Code Written by myself to initialise a candleStick object with its properties. //////
candleStick::candleStick(std::string _timestamp,
                         double _open,
                         double _high,
                         double _low,
                         double _close,
                         countryType _country)
    : timestamp(_timestamp),
      open(_open),
      high(_high),
      low(_low),
      close(_close),
      country(_country)
{
}

///// Code Written by myself to converts an integer input to its corresponding country type. //////
countryType candleStick::intToCountryType(int index)
{
    // Map of integers to country types
    std::map<int, countryType> countryMap = {
        {1, countryType::AT}, {2, countryType::BE}, {3, countryType::BG}, {4, countryType::CH}, {5, countryType::CZ}, {6, countryType::DE}, {7, countryType::DK}, {8, countryType::EE}, {9, countryType::ES}, {10, countryType::FI}, {11, countryType::FR}, {12, countryType::GB}, {13, countryType::GR}, {14, countryType::HR}, {15, countryType::HU}, {16, countryType::IE}, {17, countryType::IT}, {18, countryType::LT}, {19, countryType::LU}, {20, countryType::LV}, {21, countryType::NL}, {22, countryType::NO}, {23, countryType::PL}, {24, countryType::PT}, {25, countryType::RO}, {26, countryType::SE}, {27, countryType::SI}, {28, countryType::SK}};

    // Find the corresponding country type
    std::map<int, countryType>::const_iterator it = countryMap.find(index);
    if (it != countryMap.end())
    {
        // Return the matching country type
        return it->second;
    }

    // Default to unknown if not found
    return countryType::unknown;
}

///// Code Written by myself to converts a country string into an integer code. //////
int candleStick::stringToCountryInteger(const std::string &country)
{
    // Map of country strings to integer codes
    std::map<std::string, int> countryMap = {
        {"AT", 1}, {"BE", 2}, {"BG", 3}, {"CH", 4}, {"CZ", 5}, {"DE", 6}, {"DK", 7}, {"EE", 8}, {"ES", 9}, {"FI", 10}, {"FR", 11}, {"GB", 12}, {"GR", 13}, {"HR", 14}, {"HU", 15}, {"IE", 16}, {"IT", 17}, {"LT", 18}, {"LU", 19}, {"LV", 20}, {"NL", 21}, {"NO", 22}, {"PL", 23}, {"PT", 24}, {"RO", 25}, {"SE", 26}, {"SI", 27}, {"SK", 28}};

    // Find the corresponding integer code
    std::map<std::string, int>::const_iterator it = countryMap.find(country);
    if (it != countryMap.end())
    {
        // Return the matching integer code
        return it->second;
    }

    // Return 0 for unknown country code
    return 0;
}

///// Code Written by myself to converts a country type into its string representation. //////
std::string candleStick::countryToString(countryType &country)
{
    // Map of country types to strings
    std::map<countryType, std::string> countryMap = {
        {countryType::AT, "AT"}, {countryType::BE, "BE"}, {countryType::BG, "BG"}, {countryType::CH, "CH"}, {countryType::CZ, "CZ"}, {countryType::DE, "DE"}, {countryType::DK, "DK"}, {countryType::EE, "EE"}, {countryType::ES, "ES"}, {countryType::FI, "FI"}, {countryType::FR, "FR"}, {countryType::GB, "GB"}, {countryType::GR, "GR"}, {countryType::HR, "HR"}, {countryType::HU, "HU"}, {countryType::IE, "IE"}, {countryType::IT, "IT"}, {countryType::LT, "LT"}, {countryType::LU, "LU"}, {countryType::LV, "LV"}, {countryType::NL, "NL"}, {countryType::NO, "NO"}, {countryType::PL, "PL"}, {countryType::PT, "PT"}, {countryType::RO, "RO"}, {countryType::SE, "SE"}, {countryType::SI, "SI"}, {countryType::SK, "SK"}, {countryType::unknown, "Unknown"}};

    // Find the corresponding string representation
    std::map<countryType, std::string>::const_iterator it = countryMap.find(country);
    if (it != countryMap.end())
    {
        // Return the matching string
        return it->second;
    }

    // Return "Unknown" if not found
    return "Unknown";
}
