#include "ticker_receiver.h"

using namespace std;

TickerReceiver::TickerReceiver() {
    exchanges["Kraken"] = new KrakenExchange();
    // exchanges["Binance"] = new BinanceExchange();
}

void TickerReceiver::subscribe(const string& exchangeName, const string& currencyPair) {
    if (exchanges.find(exchangeName) == exchanges.end()) {
        cout << "Error: Exchange " << exchangeName << " not found. Available exchanges are: ";
        for (auto& exchange : exchanges) {
            cout << exchange.first << " ";
        }
        cout << endl;
        return;
    }
    exchanges[exchangeName]->connect();
    cout << "Subscribing to " << currencyPair << " on " << exchangeName << endl;
    exchanges[exchangeName]->subscribeTicker(currencyPair);
}

void TickerReceiver::unsubscribe(const string& exchange, const string& currencyPair) {
    if (exchanges.find(exchange) == exchanges.end()) {
        cout << "Error: Exchange " << exchange << " not found. Available exchanges are: ";
        for (auto& exchange : exchanges) {
            cout << exchange.first << " ";
        }
        cout << endl;
        return;
    }
    exchanges[exchange]->unsubscribeTicker(currencyPair);
}
