#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <iostream>
// #include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class Exchange {
public:
    // Method to connect to the exchange WebSocket
    virtual void connect() = 0;

    // Method to subscribe to ticker data for a specific currency pair
    virtual void subscribeTicker(const std::string& currencyPair) = 0;

    // Method to unsubscribe from ticker data for a specific currency pair
    virtual void unsubscribeTicker(const std::string& currencyPair) = 0;
};

class KrakenExchange : public Exchange {
public:
    KrakenExchange();
    void connect() override;
    void subscribeTicker(const std::string& currencyPair) override;
    void unsubscribeTicker(const std::string& currencyPair) override;

private:
    std::string url;

    client endpoint;
    websocketpp::connection_hdl hdl;

    context_ptr onTLSInit(const char * hostname, websocketpp::connection_hdl);
    void onMessage(websocketpp::connection_hdl hdl, client::message_ptr msg);

    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
};

#endif // EXCHANGE_H
