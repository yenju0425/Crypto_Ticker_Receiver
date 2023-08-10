#include <iostream>
#include "ticker_receiver.h"

using namespace std;

TickerReceiver::TickerReceiver() {
    exchanges["Kraken"] = new KrakenExchange();
    // exchanges["Binance"] = new BinanceExchange();
}

TickerReceiver::~TickerReceiver() {
    for (const auto& exchange : exchanges) {
        delete exchange.second;
    }
    exchanges.clear();
}

Exchange* TickerReceiver::get_exchange(const string& exchangeName) {
    if (exchanges.find(exchangeName) == exchanges.end()) {
        cout << "> Error: Exchange " << exchangeName << " not found. Available exchanges: ";
        for (auto& exchange : exchanges) {
            cout << exchange.first << " ";
        }
        cout << endl;
        return nullptr;
    }

    return exchanges[exchangeName];
}

void TickerReceiver::connect(const string& exchangeName) {
    Exchange* exchange = get_exchange(exchangeName);
    if (exchange == nullptr) {
        return;
    }

    const int connectionId = exchange->connect();

    this_thread::sleep_for(chrono::seconds(2)); // Wait for 2 seconds before next command to prevent errors.

    if (connectionId >= 0) {
        cout << "> Successfully connected to " << exchangeName << " with connection id " << connectionId << endl;
        return;
    }
}

void TickerReceiver::close(const string& exchangeName, const int& id, websocketpp::close::status::value code, const string& reason) {
    Exchange* exchange = get_exchange(exchangeName);
    if (exchange == nullptr) {
        return;
    }

    exchange->close(id, code, reason);

    this_thread::sleep_for(chrono::seconds(2)); // Wait for 2 seconds before next command to prevent errors.
}

void TickerReceiver::subscribe(const string& exchangeName, const int& id, const string& currencyPair) {
    Exchange* exchange = get_exchange(exchangeName);
    if (exchange == nullptr) {
        return;
    }

    exchange->subscribe_ticker(id, currencyPair);
}

void TickerReceiver::unsubscribe(const string& exchangeName, const int& id, const string& currencyPair) {
    Exchange* exchange = get_exchange(exchangeName);
    if (exchange == nullptr) {
        return;
    }

    exchange->unsubscribe_ticker(id, currencyPair);
}

void TickerReceiver::list_open_connection_ids(const string& exchangeName) {
    Exchange* exchange = get_exchange(exchangeName);
    if (exchange == nullptr) {
        return;
    }

    vector<int> ids = exchange->get_open_connection_ids();

    stringstream ss;
    ss << "> Open connection ids for " << exchangeName << ": ";
    for (auto& id : ids) {
        ss << id << " ";
    }

    cout << ss.str() << endl;
}

void TickerReceiver::show_connection_metadata(const string& exchangeName, const int& id) {
    Exchange* exchange = get_exchange(exchangeName);
    if (exchange == nullptr) {
        return;
    }

    connection_metadata::ptr metadata = exchange->get_metadata(id);

    if (metadata) {
        cout << *metadata << endl;
    } else {
        cout << "> Unknown connection id " << id << endl;
    }
}
