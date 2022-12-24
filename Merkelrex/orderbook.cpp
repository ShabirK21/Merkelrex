#include "orderbook.h"
#include "csvreader.h"
#include <map>
#include <cmath>
#include <algorithm>

OrderBook::OrderBook(std::string filename) {
    orders = csvReader::readCSV(std::move(filename));
}

std::vector <std::string> OrderBook::getKnownProducts() {
    std::vector <std::string> products;

    std::map<std::string, bool> prodMap;

    for (OrderBookEntry &e: orders) {
        prodMap[e.product] = true;
    }

    for (auto const &e: prodMap) {
        products.push_back(e.first);
    }

    return products;
}

std::vector <OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                  std::string product,
                                                  std::string timestamp) {
    std::vector <OrderBookEntry> orders_sub;

    for (OrderBookEntry &e: orders) {
        if (e.orderType == type && e.product == product &&
            e.timestamp == timestamp) {
            orders_sub.push_back(e);
        }
    }

    return orders_sub;
}

double OrderBook::getHighPrice(std::vector <OrderBookEntry> &orders) {
    double max = orders[0].price;
    for (OrderBookEntry &e: orders) {
        if (e.price > max)
            max = e.price;
    }
    return max;
}

double OrderBook::getLowPrice(std::vector <OrderBookEntry> &orders) {
    double min = orders[0].price;
    for (OrderBookEntry &e: orders) {
        if (e.price < min)
            min = e.price;
    }
    return min;
}

double OrderBook::getStdDev(std::vector <OrderBookEntry> &orders) {
    double mean = 0;
    double sum = 0;
    double stdDev = 0;

    for (OrderBookEntry &e: orders) {
        sum += e.price;
    }

    mean = sum / orders.size();

    for (OrderBookEntry &e: orders) {
        stdDev += pow(e.price - mean, 2);
    }

    stdDev = sqrt(stdDev / orders.size());

    return stdDev;

}

double OrderBook::getAverage(std::vector <OrderBookEntry> &orders, int timestamps) {
    double avg = 0;

    for (int i = 0; i < timestamps; i++) {
        avg += orders[i].price;
    }

    avg /= timestamps;
    return avg;
}

std::string OrderBook::getEarliestTime() { return orders[0].timestamp; }

std::string OrderBook::getNextTime(std::string timestamp) {
    std::string next_timestamp = "";
    for (OrderBookEntry &e: orders) {
        if (e.timestamp > timestamp) {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
        next_timestamp = orders[0].timestamp;
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry &order) {
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector <OrderBookEntry> OrderBook::matchAskToBids(std::string product, std::string timestamp) {
    std::vector <OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    std::vector <OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);
    std::vector <OrderBookEntry> sales;

    // Sort asks from lowest to highest
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // Sort bids from highest to lowest
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDes);

    for (OrderBookEntry &ask: asks) {
        for (OrderBookEntry &bid: bids) {
            if (bid.price >= ask.price) {
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

                if (bid.username == "simuser") {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser") {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }


                if (bid.amount == ask.amount) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0;
                    break;
                }

                if (bid.amount > ask.amount) {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount -= ask.amount;
                    break;
                }

                if (bid.amount < ask.amount && bid.amount > 0) {
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount -= bid.amount;
                    bid.amount = 0;
                    continue;
                }
            }

        }
    }
    return sales;
}
