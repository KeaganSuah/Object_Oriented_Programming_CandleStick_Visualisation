#pragma once

#include <string>

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

class candleStick
{
public:
    candleStick(std::string _timestamp,
                double _open,
                double _high,
                double _low,
                double _close,
                countryType _country);

    static countryType intToCountryType(int index);

    static int stringToCountryInteger(const std::string &country);

    static std::string countryToString(countryType &country);

    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
    countryType country;
};
