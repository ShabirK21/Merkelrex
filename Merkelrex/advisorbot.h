#pragma once

#include "merkelmain.h"
#include "orderbook.h"
#include <string>
#include <vector>

class AdvisorBot {

public:
  AdvisorBot();

  /** Called to start the advisorbot*/
  void init();

private:
  /** Captures and returns user input*/
  std::string getUserInput();
  /** Prints start menu*/
  void startScreen();
  /** Processes user input and decides command to executes*/
  void processUserInput(const std::vector<std::string> &userInput);
  /** Formats system output*/
  void AdvisorBotFormatting(const std::string &advisorBotOutput);
  /** Checks if user input is valid*/
  bool checkIfValid(const std::string &currencyPair,
                    const std::string &orderBookType);
  /** Exponential moving average to predict price*/
  double EMA(const std::string &MaxOrMin, const std::string &currencyPair,
             const std::string &orderBookType);
  /** Checks if user input is valid*/
  bool checkIfInt(const std::string &input);
  /** Generates order book entries from user input*/
  std::vector<OrderBookEntry> generateEntry(const std::string &orderBookType,
                                            const std::string &currencyPair);

  double average(const std::string &orderBookType,
                 const std::string &currencyPair, int timestamps);

  double stdDev(std::vector<OrderBookEntry> orders);

  bool runAdvisorBot;

  std::string currentTime;

  std::vector<std::string> history;
  // Calls existing instance of MerkelMain orderbook
  OrderBook OB = app.getOB();
};
