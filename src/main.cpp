#include "ticker_receiver.h"

using namespace std;

int main() {
    TickerReceiver tickerReceiver;

    string input;

    while (true) {
        cout << "Enter Command: ";
        getline(cin, input);

        if (input == "quit") {
            break;
        }
        else if (input == "help") {
            cout
                << "\nCommand List:\n"
                << "connect <exchange name>\n"
                << "subscribe <exchange name> <connection id> <currency pair>\n"
                << "unsubscribe <exchange name> <connection id> <currency pair>\n"
                << "close <exchange name> <connection id> [<close code:default=1000>] [<close reason>]\n"
                << "list <exchange name>\n"
                << "show <exchange name> <connection id>\n"
                << "help: Display this help text\n"
                << "quit: Exit the program\n"
                << endl;
        }
        else if (input.substr(0,7) == "connect") {
            string exchange_name = input.substr(8);
            tickerReceiver.connect(exchange_name);
        }
        else if (input.substr(0, 9) == "subscribe") {
            stringstream ss(input);
            string cmd;
            string exchangeName;
            int id;
            string currencyPair;

            ss >> cmd >> exchangeName >> id >> currencyPair;

            tickerReceiver.subscribe(exchangeName, id, currencyPair);
        }
        else if (input.substr(0, 11) == "unsubscribe") {
            stringstream ss(input);
            string cmd;
            string exchangeName;
            int id;
            string currencyPair;

            ss >> cmd >> exchangeName >> id >> currencyPair;

            tickerReceiver.unsubscribe(exchangeName, id, currencyPair);
        }
        else if (input.substr(0, 5) == "close") {
            stringstream ss(input);
            string cmd;
            string exchangeName;
            int id;
            int close_code = websocketpp::close::status::normal;
            string reason;

            ss >> cmd >> exchangeName >> id >> close_code >> reason;

            tickerReceiver.close(exchangeName, id, static_cast<websocketpp::close::status::value>(close_code), reason);
        }
        else if (input.substr(0,4) == "list") {
            string exchangeName = input.substr(5);
            tickerReceiver.list_open_connection_ids(exchangeName);
        }
        else if (input.substr(0,4) == "show") {
            stringstream ss(input);
            string cmd;
            string exchangeName;
            int id;

            ss >> cmd >> exchangeName >> id;

            tickerReceiver.show_connection_metadata(exchangeName, id);
        }
        else {
            cout << "> Unrecognized Command" << endl;
        }
    }

    return 0;
}
