#include "exchange.h"

using namespace std;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef map<int, connection_metadata::ptr> con_list;
typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

Exchange::~Exchange() {
}

string Exchange::get_name() const {
    return m_name;
}

KrakenExchange::KrakenExchange() {
    m_name = "Kraken";
    m_url = "wss://ws.kraken.com";
    m_next_id = 0;

    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.clear_error_channels(websocketpp::log::elevel::all);
    m_endpoint.init_asio();
    m_endpoint.start_perpetual();
    m_endpoint.set_tls_init_handler(bind(&KrakenExchange::on_tls_init, this, ::_1));

    m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint); // This is necessary to run the WebSocket event loop in a separate thread.
}

KrakenExchange::~KrakenExchange() {
    m_endpoint.stop_perpetual();
        
    for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
        if (it->second->get_status() != "Open") {
            continue;
        }
        
        websocketpp::lib::error_code ec;
        m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
        if (ec) {
            cout << "> Error closing connection " << it->second->get_id() << ": " << ec.message() << endl;
        }
    }
    
    m_thread->join();
}

int KrakenExchange::connect() {
    websocketpp::lib::error_code ec;
    client::connection_ptr con = m_endpoint.get_connection(m_url, ec);
    if (ec) {
        cout << "> Connect initialization error: " << ec.message() << endl;
        return -1;
    }

    int new_id = m_next_id++;
    connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(new_id, con->get_handle(), m_url, this);
    m_connection_list[new_id] = metadata_ptr;
    con->set_open_handler(bind( // triggered when connection is successfully established
        &connection_metadata::on_open,
        metadata_ptr,
        &m_endpoint,
        ::_1
    ));
    con->set_fail_handler(bind( // triggered when the connection fails, such as when the server is unreachable or the internet connection is lost
        &connection_metadata::on_fail,
        metadata_ptr,
        &m_endpoint,
        ::_1
    ));
    con->set_close_handler(bind( // triggered when the connection is closed
        &connection_metadata::on_close,
        metadata_ptr,
        &m_endpoint,
        ::_1
    ));
    con->set_message_handler(bind(
        &connection_metadata::on_message,
        metadata_ptr,
        ::_1,
        ::_2
    ));

    m_endpoint.connect(con);

    return new_id;
}

void KrakenExchange::close(int id, websocketpp::close::status::value code, string reason) {
    websocketpp::lib::error_code ec;
    
    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        cout << "> No connection found with id " << id << endl;
        return;
    }
    
    m_endpoint.close(metadata_it->second->get_hdl(), code, reason, ec);
    if (ec) {
        cout << "> Error initiating close: " << ec.message() << endl;
    }
}

void KrakenExchange::subscribe_ticker(const int& id, const string& currencyPair) {
    websocketpp::lib::error_code ec;

    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        cout << "> No connection found with id " << id << endl;
        return;
    }

    string payload = "{\"event\":\"subscribe\",\"pair\":[\"" + currencyPair + "\"],\"subscription\":{\"name\":\"ticker\"}}";

    m_endpoint.send(metadata_it->second->get_hdl(), payload, websocketpp::frame::opcode::text, ec);
    if (ec) {
        cout << "> Error sending message: " << ec.message() << endl;
        return;
    }

    metadata_it->second->record_sent_message(payload);
}

void KrakenExchange::unsubscribe_ticker(const int& id, const string& currencyPair) {
    websocketpp::lib::error_code ec;

    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        cout << "> No connection found with id " << id << endl;
        return;
    }

    string payload = "{\"event\":\"unsubscribe\",\"pair\":[\"" + currencyPair + "\"],\"subscription\":{\"name\":\"ticker\"}}";

    m_endpoint.send(metadata_it->second->get_hdl(), payload, websocketpp::frame::opcode::text, ec);
    if (ec) {
        cout << "> Error sending message: " << ec.message() << endl;
        return;
    }

    metadata_it->second->record_sent_message(payload);
}

vector<int> KrakenExchange::get_open_connection_ids() const {
    vector<int> ids;
    for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
        if (it->second->get_status() == "Open") {
            ids.push_back(it->first);
        }
    }
    return ids;
}

connection_metadata::ptr KrakenExchange::get_metadata(int id) const {
    con_list::const_iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end()) {
        return connection_metadata::ptr();
    } else {
        return metadata_it->second;
    }
}

context_ptr KrakenExchange::on_tls_init(websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
    } catch (exception &e) {
        cout << "Error in context pointer: " << e.what() << endl;
    }
    return ctx;
}
