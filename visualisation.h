#pragma once

#include <vector>
#include <string>
#include "candleStick.h"
#include "filterCandlestick.h"

class visualisation
{
public:
    visualisation();

    void displayChunks(const std::vector<candleStick> &vectorOfCandlesticks,
                       const filterCandlestick &UserFiltered,
                       size_t chunkSize = 40);

private:
    void displayHeader(const filterCandlestick &UserFiltered);
    void displayChunk(const std::vector<candleStick> &chunk,
                      const filterCandlestick &UserFiltered);
    void displayTimestamps(const std::vector<candleStick> &chunk);
};
