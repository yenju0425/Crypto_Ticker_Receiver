#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <iostream>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp> // #include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include "connection_metadata.h"

class Exchange {
public:
    virtual int connect() = 0;
    //virtual void close() = 0;
    virtual void subscribeTicker(const int& id, const std::string& currencyPair) = 0;
    virtual void unsubscribeTicker(const int& id, const std::string& currencyPair) = 0;

    virtual connection_metadata::ptr get_metadata(int id) const = 0;
};

class KrakenExchange : public Exchange {
public:
    KrakenExchange();
    ~KrakenExchange();
    int connect() override;
    //void close() override;
    void subscribeTicker(const int& id, const std::string& currencyPair) override;
    void unsubscribeTicker(const int& id, const std::string& currencyPair) override;
    connection_metadata::ptr get_metadata(int id) const override;

private:
    typedef std::map<int,connection_metadata::ptr> con_list;
    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
    typedef websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_ptr;
    typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

    context_ptr onTLSInit(const char* hostname, websocketpp::connection_hdl);

    std::string m_url;

    client m_endpoint;
    thread_ptr m_thread;

    con_list m_connection_list;
    int m_next_id;
};

#endif // EXCHANGE_H
