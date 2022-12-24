#pragma once

#include "csvreader.h"
#include "orderbookentry.h"
#include <string>
#include <vector>

class OrderBook {
public:
    /** construct, reading a csv data file*/
    OrderBook(std::string filename);

    /** return vector of all known products in the dataset*/
    std::vector <std::string> getKnownProducts();

    /** return vectors of Orders according to the sent filters*/
    std::vector <OrderBookEntry> getOrders(OrderBookType type, std::string product,
                                           std::string timestamp);

    /** return the earliest time in the orderbook*/
    std::string getEarliestTime();

    /** return the next time after the sent time in the orderbook
     * If there is no next timestamp, wrap around to the start*/
    std::string getNextTime(std::string timestamp);

    void insertOrder(OrderBookEntry &order);

    std::vector <OrderBookEntry> matchAskToBids(std::string product, std::string timestamp);

    static double getHighPrice(std::vector <OrderBookEntry> &orders);

    static double getLowPrice(std::vector <OrderBookEntry> &orders);

    static double getStdDev(std::vector <OrderBookEntry> &orders);

    static double getAverage(std::vector <OrderBookEntry> &orders, int timestamps);

private:
    std::vector <OrderBookEntry> orders;
};

