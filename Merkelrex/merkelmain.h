#pragma once

#include "orderbook.h"
#include "orderbookentry.h"
#include "wallet.h"

class MerkelMain {
public:
  MerkelMain();

  /** Call this to start the sim */
  void init();

  OrderBook getOB();

  static MerkelMain appInst;

private:
  void printMenu();

  void printHelp();

  void printMarketStats();

  void enterAsk();

  void enterBid();

  void printWallet();

  int getUserOption();

  void getNextTimeFrame();

  void processUserOption(int userOption);

  std::string currentTime;

  Wallet wallet;

  OrderBook orderBook{"data/20200601.csv"};
};

// Declare global instance of MerkelMain app
extern MerkelMain app;
