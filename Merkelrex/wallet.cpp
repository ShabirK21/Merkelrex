#include "wallet.h"
#include "csvreader.h"
#include <iostream>

Wallet::Wallet() {

}

void Wallet::insertCurrency(std::string type, double amount) {
    double balance;
    if (amount < 0) {
        throw;
    }
    if (currencies.count(type) == 0) {
        balance = 0;
    } else {
        balance = currencies[type];
    }
    balance += amount;
    currencies[type] = balance;
}

bool Wallet::removeCurrency(std::string type, double amount) {
    double balance;
    if (amount < 0) {
        return false;
    }
    if (currencies.count(type) == 0) {
        return false;
    } else {
        if (hasCurrency(type, amount)) { // check if wallet contains enough currency
            currencies[type] -= amount;
            std::cout << "Removing " << amount << " " << type << " from wallet" << std::endl;
            return true;
        } else { // have currency but not enough
            return false;
        }
    }

}

std::string Wallet::toString() {
    std::string s;
    for (std::pair<std::string, double> pair: currencies) {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + ": " + std::to_string(amount) + "\n";
    }
    return s;
}

bool Wallet::hasCurrency(std::string type, double amount) {
    if (currencies.count(type) == 0) return false;

    else return true;

}

bool Wallet::canFulfillOrder(OrderBookEntry &order) {
    std::vector <std::string> currs = csvReader::tokenize(order.product, '/');
    // ask
    if (order.orderType == OrderBookType::ask) {
        double amount = order.amount;
        std::string currency = currs[0];
        std::cout << "Wallet::canFulfillOrder " << currency << " " << amount << std::endl;
        return hasCurrency(currency, amount);
        //bid
    } else if (order.orderType == OrderBookType::bid) {
        double amount = order.amount * order.price;
        std::string currency = currs[1];
        std::cout << "Wallet::canFulfillOrder " << currency << " " << amount << std::endl;
        return hasCurrency(currency, amount);
    }
    return false;

}

void Wallet::processSale(OrderBookEntry &sale) {
    std::vector <std::string> currs = csvReader::tokenize(sale.product, '/');
    // incoming, lose currency a and gain currency b
    if (sale.orderType == OrderBookType::asksale) {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;

        // outgoing, lose currency b and gain currency a
    } else if (sale.orderType == OrderBookType::bidsale) {
        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}


