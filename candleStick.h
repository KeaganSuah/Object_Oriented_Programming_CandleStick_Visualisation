#pragma once

#include <string>

///// Enum representing different countries and an unknown type //////
enum class countryType
{
    AT,
    BE,
    BG,
    CH,
    CZ,
    DE,
    DK,
    EE,
    ES,
    FI,
    FR,
    GB,
    GR,
    HR,
    HU,
    IE,
    IT,
    LT,
    LU,
    LV,
    NL,
    NO,
    PL,
    PT,
    RO,
    SE,
    SI,
    SK,
    unknown
};
///// Class representing a candlestick with financial data and a country type //////
class candleStick
{
public:
    // Constructor to initialise a candleStick object with its properties
    candleStick(std::string _timestamp,
                double _open,
                double _high,
                double _low,
                double _close,
                countryType _country);

    // Converts an integer to a country type
    static countryType intToCountryType(int index);

    // Converts a country string to its integer code
    static int stringToCountryInteger(const std::string &country);

    // Converts a country type to its string representation
    static std::string countryToString(countryType &country);

    // Properties of CandleStick Class
    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
    countryType country;
};
