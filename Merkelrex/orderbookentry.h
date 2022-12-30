#pragma once

#include <string>

enum class OrderBookType {
    bid, ask, unknown, bidsale, asksale
};

class OrderBookEntry {
public:
    OrderBookEntry(double _price, double _amount, std::string _timestamp,
                   std::string _product, OrderBookType _orderType, std::string username = "dataset");

    static OrderBookType stringToOBT(const std::string &s);

    static bool compareByTimestamp(OrderBookEntry &e1, OrderBookEntry &e2) {
        return e1.timestamp < e2.timestamp;
    }

    static bool compareByPriceAsc(OrderBookEntry &e1, OrderBookEntry &e2) {
        return e1.price < e2.price;
    }

    static bool compareByPriceDes(OrderBookEntry &e1, OrderBookEntry &e2) {
        return e1.price > e2.price;
    }

    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
    std::string username;
};

