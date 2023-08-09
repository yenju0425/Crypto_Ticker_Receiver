#include "exchange.h"

using namespace std;

KrakenExchange::KrakenExchange() {
    m_url = "wss://ws.kraken.com";
    m_next_id = 0;

    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);  // Disables logging of all non-error events.
    m_endpoint.clear_error_channels(websocketpp::log::elevel::all);   // Disables logging of all error events.
    m_endpoint.init_asio();                                           // Initializes the Asio transport policy.
    m_endpoint.start_perpetual();                                     // Starts the m_endpoint processing thread.
    m_endpoint.set_tls_init_handler(bind(&KrakenExchange::onTLSInit, this, m_url.c_str(), ::_1));
    //m_endpoint.set_message_handler(bind(&KrakenExchange::onMessage, this, ::_1, ::_2));

    m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint); // This is necessary to run the WebSocket event loop in a separate thread.
}

KrakenExchange::~KrakenExchange() {
}

int KrakenExchange::connect() {
    websocketpp::lib::error_code ec;
    client::connection_ptr con = m_endpoint.get_connection(m_url, ec);
    if (ec) {
        std::cout << "> Connect initialization error: " << ec.message() << std::endl;
        return -1;
    }

    int new_id = m_next_id++;
    connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(new_id, con->get_handle(), m_url);
    m_connection_list[new_id] = metadata_ptr;
    con->set_tls_init_handler(websocketpp::lib::bind(
        &connection_metadata::on_tls_init,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_open_handler(websocketpp::lib::bind(
        &connection_metadata::on_open,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_fail_handler(websocketpp::lib::bind(
        &connection_metadata::on_fail,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_close_handler(websocketpp::lib::bind(
        &connection_metadata::on_close,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1
    ));
    con->set_message_handler(websocketpp::lib::bind(
        &connection_metadata::on_message,
        metadata_ptr,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2
    ));

    m_endpoint.connect(con);

    return new_id;
}

void KrakenExchange::subscribeTicker(const int& id, const string& currencyPair) {
    websocketpp::lib::error_code ec;

    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    string payload = "{\"event\":\"subscribe\",\"pair\":[\"" + currencyPair + "\"],\"subscription\":{\"name\":\"ticker\"}}";

    m_endpoint.send(metadata_it->second->get_hdl(), payload, websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "> Error sending message: " << ec.message() << std::endl;
        return;
    }
}

void KrakenExchange::unsubscribeTicker(const int& id, const string& currencyPair) {
    websocketpp::lib::error_code ec;

    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }
}

context_ptr KrakenExchange::onTLSInit(const char * hostname, websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    return ctx;
}

// void KrakenExchange::onMessage(websocketpp::connection_hdl hdl, client::message_ptr msg) {
//     if (msg->get_opcode() == websocketpp::frame::opcode::text) {
//         cout << msg->get_payload() << endl;
//     }
// }
