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
    ~TickerReceiver();

    Exchange* get_exchange(const std::string& exchangeName);

   void connect(const std::string& exchangeName);
    void close(const std::string& exchangeName, const int& id, websocketpp::close::status::value code, const std::string& reason);

    void subscribe(const std::string& exchangeName, const int& id, const std::string& currencyPair);
    void unsubscribe(const std::string& exchangeName, const int& id, const std::string& currencyPair);

    void list_open_connection_ids(const std::string& exchangeName);
    void show_connection_metadata(const std::string& exchangeName, const int& id);

private:
    std::unordered_map<std::string, Exchange*> exchanges;
};

#endif // TICKER_RECEIVER_H
