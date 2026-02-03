#include "TCPServer.hpp"
#include "Gateway.hpp"
#include "MatchingEngine.hpp"
#include "Logger.hpp"
#include "Config.hpp"
#include <iostream>
#include <signal.h>
#include <atomic>

std::atomic<bool> running(true);

void signalHandler(int signum) {
    running = false;
}

int main() {
    signal(SIGINT, signalHandler);

    Config config;
    RingBuffer<Order> orderQueue(config.orderQueueSize);
    Logger logger;

    auto tradeCallback = [&logger](const Trade& trade) {
        logger.logTrade(trade);
    };

    MatchingEngine engine(orderQueue, tradeCallback);
    Gateway gateway(orderQueue);
    TCPServer server(config.port, gateway);

    std::thread serverThread(&TCPServer::start, &server);

    std::cout << "HFT Order Matching Engine started on port " << config.port << ". Press Ctrl+C to stop." << std::endl;

    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    server.stop();
    serverThread.join();

    return 0;
}