#include <iostream>
#include <limits>

#include "merkelmain.h"
#include "csvreader.h"
#include "orderbookentry.h"
#include "advisorbot.h"


MerkelMain::MerkelMain() {}

void MerkelMain::init() {
    currentTime = orderBook.getEarliestTime();
    wallet.insertCurrency("BTC", 10.0);
    wallet.insertCurrency("ETH", 10.0);
    std::cout << "========================================================================================" << std::endl;
    std::cout << "---MMM-----MMM--EEEEEEE--RRRRR-----KK---KK--EEEEEEE--LL-----RRRRR-----EEEEEEE--XX---XX--" << std::endl;
    std::cout << "---MMMM---MMMM--EE-------RR---RRR--KK--KK---EE-------LL-----RR---RRR--EE-------XXX-XXX--" << std::endl;
    std::cout << "---MMMMM-MMMMM--EEEEEEE--RRR--RR---KKKKK----EEEEEEE--LL-----RRR--RR---EEEEEEE----XXX----" << std::endl;
    std::cout << "---MMM-M-M-MMM--EEEEEEE--RRRRR-----KKKKK----EEEEEEE--LL-----RRRRR-----EEEEEEE----XXX----" << std::endl;
    std::cout << "---MMM-----MMM--EE-------RRR--RR---KK--KKK--EE-------LL-----RRR--RR---EE-------XXX-XXX--" << std::endl;
    std::cout << "---MMM-----MMM--EEEEEEE--RRR--RRR--KK---KKK-EEEEEEE--LLLLLL-RRR--RRR--EEEEEEE-XXX---XXX-" << std::endl;
    std::cout << "========================================================================================" << std::endl;
    while (true) {
        printMenu();
        int input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu() {

    // 1 print help
    std::cout << "1: Help" << std::endl;
    // 2 print exchange stats
    std::cout << "2: Exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an ask " << std::endl;
    // 4 make a bid
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue
    std::cout << "6: Continue " << std::endl;
    // 7 advisorbot
    std::cout << "7: Advisorbot" << std::endl;
    // 8  quit
    std::cout << "8: Exit " << std::endl;

    std::cout << "========================================" << std::endl;

    std::cout << "Current time: " << currentTime << std::endl;

    std::cout << "========================================" << std::endl;

}

void MerkelMain::printHelp() {
    std::cout << "Help - your aim is to make money. Analyse the market and make "
                 "bids and offers. "
              << std::endl;
}

void MerkelMain::printMarketStats() {
    for (std::string const &p: orderBook.getKnownProducts()) {
        std::cout << "Product: " << p << std::endl;

        std::vector <OrderBookEntry> entries =
                orderBook.getOrders(OrderBookType::ask, p, currentTime);
        std::cout << "Asks: " << entries.size() << std::endl;
        std::cout << "Max asks: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min asks: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "============================" << std::endl;
    }

}

void MerkelMain::enterAsk() {
    std::string input;

    std::cout << "Mark an ask - enter the amount: product, price, amount " << std::endl;
    std::cout << "Example: ETH/BTC, 0.3, 1" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    std::vector <std::string> tokens = csvReader::tokenize(input, ',');
    if (tokens.size() != 3) {
        std::cout << "MerkelMain::enterAsk Invalid input" << std::endl;
    } else {
        try {
            OrderBookEntry OBE = csvReader::stringsToOBE(tokens[1], tokens[2], currentTime, tokens[0],
                                                         OrderBookType::ask);

            OBE.username = "simuser";
            if (wallet.canFulfillOrder(OBE)) {
                orderBook.insertOrder(OBE);
                std::cout << "Order placed" << std::endl;
            } else {
                std::cout << "Insufficient funds" << std::endl;
            }
        }
        catch (std::invalid_argument &e) {
            std::cout << "MerkelMain::enterAsk Invalid input" << std::endl;
        }
    }

    std::cout << "You entered: " << input << std::endl;
    std::cout << "============================" << std::endl;
}

void MerkelMain::enterBid() {
    std::string input;

    std::cout << "Make a bid - enter the amount: product, price, amount " << std::endl;
    std::cout << "Example: ETH/BTC, 0.3, 1" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    std::vector <std::string> tokens = csvReader::tokenize(input, ',');
    if (tokens.size() != 3) {
        std::cout << "MerkelMain::enterBid Invalid input: " << input << std::endl;
    } else {
        try {
            OrderBookEntry OBE = csvReader::stringsToOBE(tokens[1], tokens[2], currentTime, tokens[0],
                                                         OrderBookType::bid);

            OBE.username = "simuser";
            if (wallet.canFulfillOrder(OBE)) {
                orderBook.insertOrder(OBE);
                std::cout << "Order placed" << std::endl;
            } else {
                std::cout << "Insufficient funds" << std::endl;
            }
        }
        catch (std::invalid_argument &e) {
            std::cout << "MerkelMain::enterBid Invalid input: " << input << std::endl;
        }
    }

    std::cout << "You entered: " << input << std::endl;
    std::cout << "============================" << std::endl;
}

void MerkelMain::printWallet() {
    std::cout << wallet.toString();
    std::cout << "============================" << std::endl;
}

int MerkelMain::getUserOption() {
    std::string input;
    std::cout << "Choose from 1 - 8: " << std::endl;
    std::cin >> input;
    std::cout << "You chose: " << input << std::endl;
    std::cout << "============================" << std::endl;
    try {
        return std::stoi(input);
    }
    catch (std::invalid_argument &e) {
        return 0;
    }
}

void MerkelMain::getNextTimeFrame() {
    std::cout << "Next time frame" << std::endl;
    for (std::string p: orderBook.getKnownProducts()) {
        std::cout << "matching " << p << std::endl;
        std::vector <OrderBookEntry> sales = orderBook.matchAskToBids(p, currentTime);
        std::cout << "Total Sales: " << sales.size() << std::endl;
        std::cout << "============================" << std::endl;
        for (OrderBookEntry &sale: sales) {
            std::cout << "Sale Price: " << sale.price << std::endl;
            std::cout << "Sale Amount: " << sale.amount << std::endl;
            std::cout << "============================" << std::endl;
            if (sale.username == "simuser") {
                // update user wallet
                wallet.processSale(sale);
            }
        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}

OrderBook MerkelMain::getOB() {
    return orderBook;
}

void MerkelMain::processUserOption(int userOption) {
    if (userOption == 1) {
        printHelp();
    } else if (userOption == 2) {
        printMarketStats();
    } else if (userOption == 3) {
        enterAsk();
    } else if (userOption == 4) {
        enterBid();
    } else if (userOption == 5) {
        printWallet();
    } else if (userOption == 6) {
        getNextTimeFrame();
    } else if (userOption == 7) {
        AdvisorBot bot{};
        bot.init();
    } else if (userOption == 8) {
        std::cout << "Exiting program..." << std::endl;
        exit(0);
    } else {
        std::cout << "Invalid option, please choose from the provided options" << std::endl;
        std::cout << "============================" << std::endl;
    }
}

// Define global instance of app
MerkelMain app;
