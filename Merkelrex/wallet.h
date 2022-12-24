#pragma once

#include "orderbookentry.h"
#include <string>
#include <map>

class Wallet {
public:
    Wallet();

    /** insert currency to wallet */
    void insertCurrency(std::string type, double amount);

    /** check if wallet contains amount of currency */
    bool hasCurrency(std::string type, double amount);

    /** remove amount of currency from wallet*/
    bool removeCurrency(std::string type, double amount);

    /** checks if wallet can fulfill order*/
    bool canFulfillOrder(OrderBookEntry &order);

    /** update contents of wallet*/
    /** assumes order was made by owner of wallet*/
    void processSale(OrderBookEntry &sale);

    /** generate a string representation of the wallet*/
    std::string toString();


private:
    std::map<std::string, double> currencies;

};

