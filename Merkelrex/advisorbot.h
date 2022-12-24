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

    bool runAdvisorBot;

    std::string currentTime;

    std::vector <std::string> history;

    bool checkIfInt(std::string input);

    bool checkIfValid(std::string currencyPair, std::string orderBookType);

    OrderBook OB = app.getOB();

    std::string statistics(std::string orderBookType, std::string currencyPair, std::string command, int timestamps = 0);

    void predictMarketPrice(std::string MaxOrMin, std::string currencyPair, std::string orderBookType);
};


