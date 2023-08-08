#include "ticker_receiver.h"

int main() {
    TickerReceiver tickerReceiver;
    tickerReceiver.subscribe("Kraken", "ETH/USDT");

    // To demonstrate unsubscription, uncomment the line below:
    // tickerReceiver.unsubscribe("Kraken", "ETH/USDT");


   // Wait for 1 second before sending the payload
    std::this_thread::sleep_for(std::chrono::seconds(30));


    return 0;
}
