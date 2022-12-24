#pragma once

#include "orderbookentry.h"
#include <string>
#include <vector>

class csvReader {
public:
    csvReader();

    static std::vector <OrderBookEntry> readCSV(std::string csvFilename);

    static std::vector <std::string> tokenize(std::string csvLine, char separator);

    static OrderBookEntry
    stringsToOBE(std::string price, std::string amount, std::string timestamp, std::string product,
                 OrderBookType OrderBookType);


private:
    static OrderBookEntry stringsToOBE(std::vector <std::string> tokens);
};

