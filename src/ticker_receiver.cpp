#include <iostream>
#include "ticker_receiver.h"

using namespace std;

TickerReceiver::TickerReceiver() {
    exchanges["Kraken"] = new KrakenExchange();
    // exchanges["Binance"] = new BinanceExchange();
}

Exchange* TickerReceiver::getExchange(const std::string& exchangeName) {
    if (exchanges.find(exchangeName) == exchanges.end()) {
        cout << "Error: Exchange " << exchangeName << " not found. Available exchanges: ";
        for (auto& exchange : exchanges) {
            cout << exchange.first << " ";
        }
        cout << endl;
        return nullptr;
    }

    return exchanges[exchangeName];
}

void TickerReceiver::connect(const string& exchangeName) {
    Exchange* exchange = getExchange(exchangeName);
    if (exchange == nullptr) {
        return;
    }

    const int connectionId = exchange->connect();
    cout << "Successfully connected to " << exchangeName << " with connection id " << connectionId << endl;
}

void TickerReceiver::close(const string& exchangeName) {

}

void TickerReceiver::showAllConnectionIds(const string& exchangeName) {
    
}

void TickerReceiver::subscribe(const string& exchangeName, const int& id, const string& currencyPair) {
    Exchange* exchange = getExchange(exchangeName);
    if (exchange == nullptr) {
        return;
    }

    exchange->subscribeTicker(id, currencyPair);
}

void TickerReceiver::unsubscribe(const string& exchangeName, const int& id, const string& currencyPair) {
    Exchange* exchange = getExchange(exchangeName);
    if (exchange == nullptr) {
        return;
    }

    exchange->unsubscribeTicker(id, currencyPair);
}
