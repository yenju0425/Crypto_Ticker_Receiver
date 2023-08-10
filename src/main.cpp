#include "ticker_receiver.h"

using namespace std;

void displayHelp() {
    cout
        << "\nCommand List:\n"
        << "connect <exchange name>                         - Connect to an exchange (e.g. Kraken)\n"
        << "subscribe <exchange name> <id> <pair>           - Subscribe to a currency pair (e.g. DOGE/USDT, ETH/USDT) on an exchange\n"
        << "unsubscribe <exchange name> <id> <pair>         - Unsubscribe from a currency pair on an exchange\n"
        << "close <exchange name> <id> [<code>] [<reason>]  - Close a connection to an exchange\n"
        << "list <exchange name>                            - List open connection IDs for an exchange\n"
        << "show <exchange name> <id>                       - Show metadata for a connection on an exchange\n"
        << "help                                            - Display this help text\n"
        << "quit                                            - Exit the program\n"
        << endl;
}

void displayNote() {
    cout
        << "\nNotes:\n"
        << "1. This ticker receiver can connect to multiple exchanges, although currently only `Kraken` is supported.\n"
        << "2. Each exchange can handle multiple connections.\n"
        << "3. Every connection can subscribe to multiple currency pairs using the subscribe/unsubscribe commands.\n"
        << "4. All logs from the same connection will be written to a file located in the same directory where you execute the program.\n"
        << "5. Connections will be automatically closed by the server after 60 seconds of inactivity. To maintain an active connection, subscribe to a currency pair.\n"
        << endl;
}

int main() {
    TickerReceiver tickerReceiver;

    string input;

    displayNote();
    displayHelp();

    while (true) {
        cout << "Enter Command: " << endl;
        getline(cin, input);

        if (input == "quit") {
            break;
        }
        else if (input == "help") {
            displayHelp();
        }
        else if (input.substr(0, 7) == "connect") {
            stringstream ss(input);
            string cmd;
            string exchangeName;

            ss >> cmd >> exchangeName;

            tickerReceiver.connect(exchangeName);
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
        else if (input.substr(0, 4) == "list") {
            stringstream ss(input);
            string cmd;
            string exchangeName;

            ss >> cmd >> exchangeName;

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
