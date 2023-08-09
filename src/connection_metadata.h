#ifndef CONNECTION_METADATA_H
#define CONNECTION_METADATA_H

#include <vector>
#include <sstream>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

class connection_metadata {
public:
    typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;

    connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri);

    context_ptr on_tls_init(client* c, websocketpp::connection_hdl hdl);
    void on_open(client* c, websocketpp::connection_hdl hdl);
    void on_fail(client* c, websocketpp::connection_hdl hdl);
    void on_close(client* c, websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg);

    websocketpp::connection_hdl get_hdl() const;
    int get_id() const;
    std::string get_status() const;
    void record_sent_message(std::string message);

    friend std::ostream & operator<< (std::ostream & out, connection_metadata const & data);

private:
    int m_id;
    websocketpp::connection_hdl m_hdl;
    std::string m_status;
    std::string m_uri;
    std::string m_server;
    std::string m_error_reason;
    std::vector<std::string> m_messages;
};

#endif // CONNECTION_METADATA_H
