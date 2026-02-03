#pragma once

#include "RingBuffer.hpp"
#include "Order.hpp"
#include <string>
#include <sstream>

class Gateway {
public:
    Gateway(RingBuffer<Order>& orderQueue) : orderQueue_(orderQueue) {}

    bool parseAndQueueOrder(const std::string& message) {
        // Simplified FIX-like parser
        std::istringstream iss(message);
        std::string token;
        std::getline(iss, token, '|');
        if (token != "ORDER") return false;

        uint64_t id;
        std::string symbol, sideStr;
        int64_t price;
        uint64_t quantity;

        std::getline(iss, token, '|'); id = std::stoull(token);
        std::getline(iss, symbol, '|');
        std::getline(iss, sideStr, '|');
        std::getline(iss, token, '|'); price = std::stoll(token);
        std::getline(iss, token, '|'); quantity = std::stoull(token);

        Side side = (sideStr == "B") ? Side::BUY : Side::SELL;
        Order order(id, symbol, side, price, quantity);
        return orderQueue_.push(order);
    }

private:
    RingBuffer<Order>& orderQueue_;
};