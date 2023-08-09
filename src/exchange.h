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
    std::string getName() const;

    virtual int connect() = 0;
    virtual void close(int id, websocketpp::close::status::value code, std::string reason) = 0;
    virtual void subscribeTicker(const int& id, const std::string& currencyPair) = 0;
    virtual void unsubscribeTicker(const int& id, const std::string& currencyPair) = 0;

    virtual connection_metadata::ptr get_metadata(int id) const = 0;

protected:
    std::string m_name;
};

class KrakenExchange : public Exchange {
public:
    typedef std::map<int,connection_metadata::ptr> con_list;
    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
    typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

    KrakenExchange();
    ~KrakenExchange();
    int connect() override;
    void close(int id, websocketpp::close::status::value code, std::string reason) override;
    void subscribeTicker(const int& id, const std::string& currencyPair) override;
    void unsubscribeTicker(const int& id, const std::string& currencyPair) override;
    connection_metadata::ptr get_metadata(int id) const override;

private:
    context_ptr onTLSInit(const char* hostname, websocketpp::connection_hdl);

    std::string m_url;

    client m_endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    con_list m_connection_list;
    int m_next_id;
};

#endif // EXCHANGE_H
