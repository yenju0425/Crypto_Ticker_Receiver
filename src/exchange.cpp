#include "exchange.h"

using namespace std;

KrakenExchange::KrakenExchange() {
    url = "wss://ws.kraken.com";

    endpoint.clear_access_channels(websocketpp::log::alevel::all);  // Disables logging of all non-error events.
    endpoint.clear_error_channels(websocketpp::log::elevel::all);   // Disables logging of all error events.
    endpoint.init_asio();                                           // Initializes the Asio transport policy.
    endpoint.start_perpetual();                                     // Starts the endpoint processing thread.
    endpoint.set_tls_init_handler(bind(&KrakenExchange::onTLSInit, this, url.c_str(), ::_1));  // Corrected line.
    endpoint.set_message_handler(bind(&KrakenExchange::onMessage, this, ::_1, ::_2));  // Sets a message handler callback to be called when a message is received by the endpoint.

    m_thread.reset(new websocketpp::lib::thread(&client::run, &endpoint));
}

void KrakenExchange::connect() {
    websocketpp::lib::error_code ec;
    client::connection_ptr con = endpoint.get_connection("wss://ws.kraken.com", ec);
    if (ec) {
        cout << "Error: " << ec.message() << endl;
        return;
    }

    hdl = con->get_handle();
    endpoint.connect(con);
    cout << "[RICKDEBUG] Connected to " << url << endl;
    //subscribeTicker("ETH/USDT");
    //endpoint.run();// already use m_thread
}

void KrakenExchange::subscribeTicker(const string& currencyPair) {
    cout << "[RICKDEBUG] Subscribing to ticker for " << currencyPair << endl;
    string payload = "{\"event\":\"subscribe\",\"pair\":[\"" + currencyPair + "\"],\"subscription\":{\"name\":\"ticker\"}}";

    // wait 1 sec
   // Wait for 1 second before sending the payload
    std::this_thread::sleep_for(std::chrono::seconds(3));


    cout << "[RICKDEBUG] payload: " << payload << endl;

    websocketpp::lib::error_code ec;
    endpoint.send(hdl, payload, websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "> Error sending message: " << ec.message() << std::endl;
        return;
    }
}

void KrakenExchange::unsubscribeTicker(const string& currencyPair) {
    string payload = "{\"event\":\"unsubscribe\",\"pair\":[\"" + currencyPair + "\"],\"subscription\":{\"name\":\"ticker\"}}";
    endpoint.send(hdl, payload, websocketpp::frame::opcode::text);
}

context_ptr KrakenExchange::onTLSInit(const char * hostname, websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    return ctx;
}

void KrakenExchange::onMessage(websocketpp::connection_hdl hdl, client::message_ptr msg) {
    if (msg->get_opcode() == websocketpp::frame::opcode::text) {
        cout << "Received: " << msg->get_payload() << endl;
        // Process the received message here
    }
}
