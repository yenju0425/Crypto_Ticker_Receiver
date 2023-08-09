#ifndef TICKER_RECEIVER_H
#define TICKER_RECEIVER_H

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

    Exchange* getExchange(const std::string& exchangeName);

    void subscribe(const std::string& exchangeName, const int& id, const std::string& currencyPair);
    void unsubscribe(const std::string& exchangeName, const int& id, const std::string& currencyPair);

    void connect(const std::string& exchangeName);
    void close(const std::string& exchangeName);

    void showAllConnectionIds(const std::string& exchange);


private:
    std::unordered_map<std::string, Exchange*> exchanges;
};

#endif // TICKER_RECEIVER_H
