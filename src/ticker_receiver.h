#ifndef TICKER_RECEIVER_H
#define TICKER_RECEIVER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include "exchange.h"

struct Ticker {
    double best_ask_price;
    double best_bid_price;
    double close_price;
    double today_high_price;
    double today_low_price;
    double today_open_price;
};

class TickerReceiver {
public:
    TickerReceiver();

    void subscribe(const std::string& exchange, const std::string& currencyPair);
    void unsubscribe(const std::string& exchange, const std::string& currencyPair);

private:
    std::unordered_map<std::string, Exchange*> exchanges;
};

#endif // TICKER_RECEIVER_H
