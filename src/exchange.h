#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <iostream>
#include <websocketpp/config/asio_client.hpp> // #include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include "connection_metadata.h"

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class Exchange {
public:
    virtual int connect() = 0;
    //virtual void close() = 0;
    virtual void subscribeTicker(const int& id, const std::string& currencyPair) = 0;
    virtual void unsubscribeTicker(const int& id, const std::string& currencyPair) = 0;
};

class KrakenExchange : public Exchange {
public:
    KrakenExchange();
    ~KrakenExchange();
    int connect() override;
    //void close() override;
    void subscribeTicker(const int& id, const std::string& currencyPair) override;
    void unsubscribeTicker(const int& id, const std::string& currencyPair) override;

private:
    typedef std::map<int,connection_metadata::ptr> con_list;
    context_ptr onTLSInit(const char* hostname, websocketpp::connection_hdl);
    //void onMessage(websocketpp::connection_hdl hdl, client::message_ptr msg);

    std::string m_url;

    client m_endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    std::map<int,connection_metadata::ptr> m_connection_list;
    int m_next_id;
};

#endif // EXCHANGE_H
