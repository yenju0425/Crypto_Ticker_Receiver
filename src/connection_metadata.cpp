#include <iostream>
#include <filesystem>
#include <chrono>
#include <ctime>
#include "connection_metadata.h"
#include "exchange.h"

using namespace std;

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

connection_metadata::connection_metadata(int id, websocketpp::connection_hdl hdl, string uri, Exchange* exchange)
    : m_id(id), m_hdl(hdl), m_status("Connecting"), m_uri(uri), m_server("N/A"), m_exchange(exchange) {

    string path = __fs::filesystem::current_path().string();
    string filename = path + "/" + exchange->get_name() + "_" + to_string(id) + ".txt";
    
    m_message_file.open(filename, ios::out);
}

context_ptr connection_metadata::on_tls_init(client* c, websocketpp::connection_hdl hdl) {
    context_ptr ctx = make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
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

void connection_metadata::on_open(client* c, websocketpp::connection_hdl hdl) {
    m_status = "Open";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");

    log_message("Connection opened");
}

void connection_metadata::on_fail(client* c, websocketpp::connection_hdl hdl) {
    m_status = "Failed";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
    m_error_reason = con->get_ec().message();

    log_message("Connection failed: " + m_error_reason);
}

void connection_metadata::on_close(client* c, websocketpp::connection_hdl hdl) {
    m_status = "Closed";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    stringstream s;
    s << "close code: " << con->get_remote_close_code() << " ("
        << websocketpp::close::status::get_string(con->get_remote_close_code())
        << "), close reason: " << con->get_remote_close_reason();
    m_error_reason = s.str();

    log_message("Connection closed: " + m_error_reason);
}

void connection_metadata::on_message(websocketpp::connection_hdl, client::message_ptr msg) {
    if (msg->get_opcode() == websocketpp::frame::opcode::text) {
        if (msg->get_payload() != "{\"event\":\"heartbeat\"}") {
            log_message("<< " + msg->get_payload());
        }
    } else {
        m_messages.push_back("<< " + websocketpp::utility::to_hex(msg->get_payload()));
    }
}

websocketpp::connection_hdl connection_metadata::get_hdl() const {
    return m_hdl;
}

int connection_metadata::get_id() const {
    return m_id;
}

string connection_metadata::get_status() const {
    return m_status;
}

void connection_metadata::record_sent_message(string message) {
    log_message(">> " + message);
    m_messages.push_back(">> " + message);
}

void connection_metadata::log_message(const string& message) {
    if (m_message_file.is_open()) {
        auto now = chrono::system_clock::now();
        time_t time_now = chrono::system_clock::to_time_t(now);

        char timestamp_buffer[80];
        strftime(timestamp_buffer, sizeof(timestamp_buffer), "[%Y-%m-%d %H:%M:%S] ", localtime(&time_now));

        cout << timestamp_buffer << message << endl;
        m_message_file << timestamp_buffer << message << endl;
        m_message_file.flush();
    } else {
        cout << "Error: file is not open" << endl;
    }
}

ostream & operator<< (ostream & out, connection_metadata const & data) {
    out << "> URI: " << data.m_uri << "\n"
        << "> Status: " << data.m_status << "\n"
        << "> Remote Server: " << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
        << "> Error/close reason: " << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason) << "\n";
    out << "> Messages Processed: (" << data.m_messages.size() << ") \n";

    vector<string>::const_iterator it;
    for (it = data.m_messages.begin(); it != data.m_messages.end(); ++it) {
        out << *it << "\n";
    }

    return out;
}
