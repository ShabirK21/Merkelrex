#pragma once

#include <string>
#include <vector>
#include "merkelmain.h"
#include "orderbook.h"

class AdvisorBot {

public:

    AdvisorBot();

    /** Called to start the advisorbot*/
    void init();

private:

    std::string getUserInput();

    void startScreen();

    void processUserInput(std::vector <std::string> &userInput);

    void AdvisorBotFormatting(std::string advisorBotOutput);

    bool checkIfValid(std::string currencyPair, std::string orderBookType);

    double EMA(std::string MaxOrMin, std::vector <OrderBookEntry> entries);

    bool checkIfInt(std::string input);

    std::vector<OrderBookEntry> generateEntry (std::string orderBookType, std::string currencyPair);

    bool runAdvisorBot;

    std::string currentTime;

    std::vector <std::string> history;

    OrderBook OB = app.getOB();





};


