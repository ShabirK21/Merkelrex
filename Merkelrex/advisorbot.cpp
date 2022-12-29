#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <fstream>

#include "advisorbot.h"
#include "csvreader.h"
#include "orderbook.h"
#include "orderbookentry.h"

AdvisorBot::AdvisorBot() {};

void AdvisorBot::init() {
    currentTime = OB.getEarliestTime();
    runAdvisorBot = true;
    while (runAdvisorBot) {
        startScreen();
        std::string input = getUserInput();
        std::vector <std::string> tokens = csvReader::tokenize(input, ' ');
        processUserInput(tokens);
    }
}

void AdvisorBot::startScreen() {
    AdvisorBotFormatting("Please enter a command or type 'help' for a list of commands");
}

std::string AdvisorBot::getUserInput() {
    std::string userInput;
    std::cout << "user> ";
    std::getline(std::cin >> std::ws, userInput);
    return userInput;

}

void AdvisorBot::AdvisorBotFormatting(std::string AdvisorBotOutput) {
    std::string systemUsername = "advisorbot> ";
    std::cout << systemUsername << AdvisorBotOutput << std::endl;
}

bool AdvisorBot::checkIfValid(std::string currencyPair, std::string orderBookType) {
    for (std::string const &p: OB.getKnownProducts()) {
        if (currencyPair == p && (orderBookType == "bid" || orderBookType == "ask")) {
            return true;
        }
    }
    AdvisorBotFormatting("Invalid currency pair or order book type");
    return false;
}

std::vector<OrderBookEntry> AdvisorBot::generateEntry(std::string currencyPair, std::string orderBookType) {
    OrderBookType OBT = OrderBookEntry::stringToOBT(orderBookType);
    std::vector<OrderBookEntry> entries = OB.getOrders(OBT, currencyPair, currentTime);
    return entries;
}

double AdvisorBot::EMA(std::string MaxOrMin, std::vector <OrderBookEntry> entries) {
    int period = 10;
    double currentPrice;
    double previousPrice;
    std::vector<double> EMA;
    double constant = 2 / (period + 1);
    double finalEMA;

    for(int i = 0; i < period; i++){
        if (i == 0){
            currentPrice = entries[i].price;
        }
        else{
            previousPrice = currentPrice;
            currentPrice = entries[i].price;
            EMA.push_back((constant*currentPrice)+((1-constant)*previousPrice));
        }
    }
    if (MaxOrMin == "max") {
        double max = *std::max_element(EMA.begin(), EMA.end());
        finalEMA = max;
    } else if (MaxOrMin == "min") {
        double min = *std::min_element(EMA.begin(), EMA.end());
        finalEMA = min;
    }

    return finalEMA;
}

bool AdvisorBot::checkIfInt(std::string input) {
    for (int i = 0; i < input.length(); i++) {
        if (isdigit(input[i])) {
            return true;
        }
    }
    AdvisorBotFormatting("Please enter a valid timestamp");
    return false;
}

void AdvisorBot::processUserInput(std::vector <std::string> &userInput) {
    std::string output;
    if (userInput.size() == 1) {
        std::string command = userInput[0];
        // output available commands
        if (command == "help") {
            AdvisorBotFormatting("commands available: help <cmd>, avg, prod, min, max, std, predict, time, step, history, save history, main");
        }
            // output available products
        else if (command == "prod") {
            AdvisorBotFormatting("Available products: ");
            for (std::string const &p: OB.getKnownProducts()) {
                AdvisorBotFormatting(p);
            }
        }
            // output current timestamp
        else if (command == "time") {
            std::string &CT = currentTime;
            AdvisorBotFormatting(CT);
        }
            // move on to next timestamp
        else if (command == "step") {
            currentTime = OB.getNextTime(currentTime);
            AdvisorBotFormatting("now at " + currentTime);
        }
            // back to main menu
        else if (command == "main") {
            runAdvisorBot = false;
        }
            // prints the user commands, and it's output
        else if (command == "history") {
            AdvisorBotFormatting("====================================");
            for (std::string const &h: history) {
                AdvisorBotFormatting(h);
            }
            AdvisorBotFormatting("====================================");
        }
        else {
            AdvisorBotFormatting("Invalid input, please enter a valid command");
        }

    } else if (userInput.size() == 2) {
        bool help = userInput[0] == "help";
        std::string command = userInput[1];
        // tell user how to use average command
        if (help && command == "avg") {
            AdvisorBotFormatting("e.g: avg ETH/BTC bid 10 -> average ETH/BTC bid over last 10 timestamps");
        }// tell user how to use minimum command
        else if (help && command == "min") {
            AdvisorBotFormatting("e.g min ETH/BTC ask/bid -> minimum ETH/BTC ask/bid");
        }// tell user how to use maximum command
        else if (help && command == "max") {
            AdvisorBotFormatting("e.g max ETH/BTC ask/bid -> maximum ETH/BTC ask/bid");
        }// tell user how to use standard deviation command
        else if (help && command == "std") {
            AdvisorBotFormatting("e.g std ETH/BTC ask/bid -> standard deviation of ETH/BTC ask/bid");
        }// tells user how to use predict command
        else if (help && command == "predict") {
            AdvisorBotFormatting("e.g predict ETH/BTC ask/bid max/min -> predicts the max/min price of ETH/BTC ask/bid");
        } // tells user purpose of prod command
        else if (help && command == "prod") {
            AdvisorBotFormatting("e.g prod -> lists all available products");
        }// tells user purpose of time command
        else if (help && command == "time") {
            AdvisorBotFormatting("e.g time -> displays the current timestamp");
        } // tells user purpose of step command
        else if (help && command == "step") {
            AdvisorBotFormatting("e.g step -> moves to the next timestamp");
        } else if (help && command == "history") {
            AdvisorBotFormatting("e.g history -> displays all previous user inputs and their output");
            AdvisorBotFormatting("e.g save history -> saves all previous user inputs and their output to a file");
        } else if (help && command == "main") {
            AdvisorBotFormatting("returns to the main menu");
        }// writes the user commands, and it's output to a file
        else if (userInput[0] == "save" && command == "history") {
            std::ofstream file;
            file.open("data/history.txt");
            for (std::string const &h: history) {
                file << h << std::endl;
            }
            file.close();
            AdvisorBotFormatting("History saved to data/history.txt");
        } else {
            AdvisorBotFormatting("Invalid input, please enter a valid command");
        }

    } else if (userInput.size() == 3) {
        std::string command = userInput[0];
        std::string currencyPair = userInput[1];
        std::string orderBookType = userInput[2];

        // convert prodcut to uppercase
        for (int i = 0; i < currencyPair.length(); i++) {
            currencyPair[i] = toupper(currencyPair[i]);
        }

        // minimum
        if (command == "min") {
            if (checkIfValid(currencyPair, orderBookType)) {
               std::vector<OrderBookEntry> entries = generateEntry(currencyPair, orderBookType);
               double min = OrderBook::getLowPrice(entries);
                output = "The " + command + " " + orderBookType + " for " +
                        currencyPair + " is " + std::to_string(min);
               AdvisorBotFormatting(output);
            }
        } // maximum
        else if (command == "max") {
            if (checkIfValid(currencyPair, orderBookType)) {
                std::vector<OrderBookEntry> entries = generateEntry(currencyPair, orderBookType);
                double max = OrderBook::getHighPrice(entries);
                output = "The " + command + " " + orderBookType + " for " +
                        currencyPair + " is " + std::to_string(max);
                AdvisorBotFormatting(output);
            }
        } // standard deviation
        else if (command == "std") {
            if (checkIfValid(currencyPair, orderBookType)) {
                std::vector<OrderBookEntry> entries = generateEntry(currencyPair, orderBookType);
                double std = OrderBook::getStdDev(entries);
                output = "The " + command + " " + orderBookType + " for " +
                        currencyPair + " is " + std::to_string(std);
                AdvisorBotFormatting(output);
            }
        } else {
            AdvisorBotFormatting("Invalid input, please enter a valid command");
        }

    } else if (userInput.size() == 4) {
        std::string command = userInput[0];

        // average
        if (command == "avg") {
            std::string currencyPair = userInput[1];
            std::string orderBookType = userInput[2];
            std::string timestamps = userInput[3];
            // convert product to uppercase
            for (int i = 0; i < currencyPair.length(); i++) {
                currencyPair[i] = toupper(currencyPair[i]);
            }
            if (checkIfValid(currencyPair, orderBookType)) {
                if (checkIfInt(timestamps)) {
                    std::vector<OrderBookEntry> entries = generateEntry(currencyPair, orderBookType);
                    double averageVal = OrderBook::getAverage(entries, std::stoi(timestamps));
                    output = "The " + command + " " + orderBookType + " for " + currencyPair + " over the last "
                             + timestamps + " timestamps is " + std::to_string(averageVal);
                    AdvisorBotFormatting(output);
                }
            }
        }// predict using exponential moving average
        else if (command == "predict") {
            std::string MaxOrMin = userInput[1];
            std::string currencyPair = userInput[2];
            std::string orderBookType = userInput[3];
            // convert prodcut to uppercase
            for (int i = 0; i < currencyPair.length(); i++) {
                currencyPair[i] = toupper(currencyPair[i]);
            }
            if (MaxOrMin == "max" || MaxOrMin == "min") {
                if (checkIfValid(currencyPair, orderBookType)) {
                    history.push_back(command + " " + MaxOrMin + " " + currencyPair + " " + orderBookType);
                    std::vector<OrderBookEntry> entries = generateEntry(currencyPair, orderBookType);
                    double prediction = EMA(MaxOrMin, entries);
                    output = "The predicted " + MaxOrMin + orderBookType + " for " +
                            currencyPair + " in the next 10 timestamps is " + std::to_string(prediction);
                    AdvisorBotFormatting(output);
                    history.push_back(output);
                }
            } else {
                AdvisorBotFormatting("Please enter either max or min");
            }
        } else {
            AdvisorBotFormatting("Invalid input, please enter a valid command");
        }
    }

}

