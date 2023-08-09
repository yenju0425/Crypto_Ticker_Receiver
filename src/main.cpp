#include "ticker_receiver.h"

int main() {
    TickerReceiver tickerReceiver;

    tickerReceiver.connect("Kraken");

   // Wait for 1 second before sending the payload
    //std::this_thread::sleep_for(std::chrono::seconds(1));

//     tickerReceiver.connect("Kraken");

//    // Wait for 1 second before sending the payload
//     std::this_thread::sleep_for(std::chrono::seconds(1));


//     tickerReceiver.connect("Kraken");

   // Wait for 1 second before sending the payload
    std::this_thread::sleep_for(std::chrono::seconds(2));

    tickerReceiver.connect("Kraken");


    std::this_thread::sleep_for(std::chrono::seconds(2));

    tickerReceiver.subscribe("Kraken", 0, "ETH/USDT");

    tickerReceiver.subscribe("Kraken", 1, "DOGE/USDT");
    // tickerReceiver.subscribe("Kraken", 1, "BTC/USDT");
    // tickerReceiver.subscribe("Kraken", 2, "ETH/USDT");

    // To demonstrate unsubscription, uncomment the line below:
    // tickerReceiver.unsubscribe("Kraken", "ETH/USDT");


   // Wait for 1 second before sending the payload
    std::this_thread::sleep_for(std::chrono::seconds(300));


    return 0;
}
