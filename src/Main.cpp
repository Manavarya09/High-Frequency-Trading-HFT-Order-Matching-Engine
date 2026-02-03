#include "TCPServer.hpp"
#include "Gateway.hpp"
#include "MatchingEngine.hpp"
#include "Logger.hpp"
#include <iostream>
#include <signal.h>
#include <atomic>

std::atomic<bool> running(true);

void signalHandler(int signum) {
    running = false;
}

int main() {
    signal(SIGINT, signalHandler);

    RingBuffer<Order> orderQueue(1024);
    Logger logger;

    auto tradeCallback = [&logger](const Trade& trade) {
        logger.logTrade(trade);
    };

    MatchingEngine engine(orderQueue, tradeCallback);
    Gateway gateway(orderQueue);
    TCPServer server(8080, gateway);

    std::thread serverThread(&TCPServer::start, &server);

    std::cout << "HFT Order Matching Engine started. Press Ctrl+C to stop." << std::endl;

    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    server.stop();
    serverThread.join();

    return 0;
}