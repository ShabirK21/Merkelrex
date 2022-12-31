#include "csvreader.h"
#include <fstream>
#include <iostream>

csvReader::csvReader() {}

std::vector<OrderBookEntry> csvReader::readCSV(const std::string &csvFilename) {
  std::vector<OrderBookEntry> entries;

  std::ifstream csvFile{csvFilename};
  std::string line;

  if (csvFile.is_open()) {
    while (std::getline(csvFile, line)) {
      try {
        OrderBookEntry OBE = stringsToOBE(tokenize(line, ','));
        entries.emplace_back(OBE);
      } catch (const std::exception &e) {
        std::cout << "Error Reading" << std::endl;
        continue;
      }
    }
  } else {
    std::cout << "File not open, exiting program..." << std::endl;
    exit(0);
  }
  std::cout << "Read " << entries.size() << " entries" << std::endl;
  return entries;
};

std::vector<std::string> csvReader::tokenize(const std::string &csvLine,
                                             char separator) {
  std::vector<std::string> tokens;
  signed int start, end;
  std::string token;

  start = csvLine.find_first_not_of(separator, 0);
  do {
    end = csvLine.find_first_of(separator, start);

    if (start == csvLine.length() || start == end)
      break;

    if (end >= 0)
      token = csvLine.substr(start, end - start);

    else
      token = csvLine.substr(start, csvLine.length() - start);

    tokens.push_back(token);

    start = end + 1;

  } while (end != std::string::npos);

  return tokens;
};

OrderBookEntry csvReader::stringsToOBE(const std::vector<std::string> &tokens) {
  double price, amount;
  if (tokens.size() != 5) {
    // std::cout << "Bad Line: " << std::endl;
    throw std::exception{};
  }
  try {
    price = std::stod(tokens[3]);
    amount = std::stod(tokens[4]);
  } catch (const std::exception &e) {
    std::cout << "Bad Line: " << tokens[3] << std::endl;
    std::cout << "Bad Line: " << tokens[4] << std::endl;
  }
  OrderBookEntry OBE{price, amount, tokens[0], tokens[1],
                     OrderBookEntry::stringToOBT(tokens[2])};
  return OBE;
};

OrderBookEntry csvReader::stringsToOBE(std::string priceString,
                                       std::string amountString,
                                       std::string timestamp,
                                       std::string product,
                                       OrderBookType orderType) {

  double price, amount;
  try {
    price = std::stod(priceString);
    amount = std::stod(amountString);
  } catch (const std::exception &e) {
    std::cout << "csvReader::stringsToOBE - Bad Line: " << priceString
              << std::endl;
    std::cout << "csvReader::stringsToOBE - Bad Line: " << amountString
              << std::endl;
  }
  OrderBookEntry OBE{price, amount, timestamp, product, orderType};
  return OBE;
};
