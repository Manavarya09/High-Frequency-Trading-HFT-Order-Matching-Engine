#include "../src/RingBuffer.hpp"
#include "../src/MatchingEngine.hpp"
#include "../src/Logger.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main() {
    const int numOrders = 100000;
    RingBuffer<Order> orderQueue(1024 * 1024);
    Logger logger;

    int tradeCount = 0;
    auto callback = [&logger, &tradeCount](const Trade& t) {
        tradeCount++;
        logger.logTrade(t);
    };

    MatchingEngine engine(orderQueue, callback);

    auto start = std::chrono::high_resolution_clock::now();

    std::thread producer([&]() {
        for (int i = 0; i < numOrders; ++i) {
            Order order(i, "AAPL", (i % 2 == 0) ? Side::BUY : Side::SELL, 10000 + (i % 100), 100);
            while (!orderQueue.push(order)) {
                std::this_thread::yield();
            }
        }
    });

    producer.join();

    // Wait for processing
    std::this_thread::sleep_for(std::chrono::seconds(5));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Processed " << numOrders << " orders in " << duration.count() << " ms" << std::endl;
    std::cout << "Trades: " << tradeCount << std::endl;

    return 0;
}