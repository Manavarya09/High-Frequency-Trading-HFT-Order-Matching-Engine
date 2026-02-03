#pragma once

#include "RingBuffer.hpp"
#include "MatchingEngine.hpp"
#include <thread>
#include <fstream>
#include <atomic>

class Logger {
public:
    Logger() : running_(true), thread_(&Logger::run, this) {}
    ~Logger() {
        running_ = false;
        thread_.join();
    }

    void logTrade(const Trade& trade) {
        tradeQueue_.push(trade);
    }

private:
    void run() {
        std::ofstream file("trades.log", std::ios::app);
        while (running_) {
            auto trade = tradeQueue_.pop();
            if (trade) {
                file << "Trade: " << trade->buyOrderId << "," << trade->sellOrderId << ","
                     << trade->price << "," << trade->quantity << "," 
                     << std::chrono::duration_cast<std::chrono::microseconds>(
                         trade->timestamp.time_since_epoch()).count() << std::endl;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    }

    RingBuffer<Trade> tradeQueue_{1024};
    std::atomic<bool> running_;
    std::thread thread_;
};