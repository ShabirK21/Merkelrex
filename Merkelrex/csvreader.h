#pragma once

#include "orderbookentry.h"
#include <string>
#include <vector>
#include <string_view>

class csvReader {
public:
    csvReader();

    static std::vector <OrderBookEntry> readCSV(const std::string &csvFilename);

    static std::vector <std::string> tokenize(const std::string &csvLine, char separator);

    static OrderBookEntry
    stringsToOBE(std::string price, std::string amount, std::string timestamp, std::string product,
                 OrderBookType OrderBookType);


private:
    static OrderBookEntry stringsToOBE(const std::vector <std::string> &tokens);
};

