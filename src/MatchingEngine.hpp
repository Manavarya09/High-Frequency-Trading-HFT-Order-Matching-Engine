#pragma once

#include "Order.hpp"
#include "OrderBook.hpp"
#include "RingBuffer.hpp"
#include <vector>
#include <functional>
#include <thread>
#include <atomic>

struct Trade {
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    int64_t price;
    uint64_t quantity;
    std::chrono::high_resolution_clock::time_point timestamp;
};

class MatchingEngine {
public:
    using TradeCallback = std::function<void(const Trade&)>;

    MatchingEngine(RingBuffer<Order>& orderQueue, TradeCallback callback) 
        : orderQueue_(orderQueue), tradeCallback_(callback), running_(true), 
          thread_(&MatchingEngine::run, this) {}

    ~MatchingEngine() {
        running_ = false;
        thread_.join();
    }

    void cancelOrder(uint64_t orderId);

private:
    void run() {
        while (running_) {
            auto order = orderQueue_.pop();
            if (order) {
                addOrder(*order);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        }
    }

    void addOrder(const Order& order);
    void match();

    OrderBook orderBook_;
    RingBuffer<Order>& orderQueue_;
    TradeCallback tradeCallback_;
    std::atomic<bool> running_;
    std::thread thread_;
};