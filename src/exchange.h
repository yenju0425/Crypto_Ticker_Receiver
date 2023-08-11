#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <iostream>
#include <map>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp> // #include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include "connection_metadata.h"

class Exchange {
public:
    virtual ~Exchange();

    std::string get_name() const;

    virtual int connect() = 0;
    virtual void close(int id, websocketpp::close::status::value code, std::string reason) = 0;
    virtual void subscribe_ticker(const int& id, const std::string& currencyPair) = 0;
    virtual void unsubscribe_ticker(const int& id, const std::string& currencyPair) = 0;

    virtual connection_metadata::ptr get_metadata(int id) const = 0;
    virtual std::vector<int> get_open_connection_ids() const = 0;

protected:
    std::string m_name;
};

class KrakenExchange : public Exchange {
public:
    typedef std::map<int, connection_metadata::ptr> con_list;
    typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
    typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

    KrakenExchange();
    ~KrakenExchange();

    int connect() override;
    void close(int id, websocketpp::close::status::value code, std::string reason) override;
    void subscribe_ticker(const int& id, const std::string& currencyPair) override;
    void unsubscribe_ticker(const int& id, const std::string& currencyPair) override;

    connection_metadata::ptr get_metadata(int id) const override;
    std::vector<int> get_open_connection_ids() const override;

private:
    context_ptr on_tls_init(websocketpp::connection_hdl);

    std::string m_url;

    client m_endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    con_list m_connection_list;
    int m_next_id;
};

#endif // EXCHANGE_H
