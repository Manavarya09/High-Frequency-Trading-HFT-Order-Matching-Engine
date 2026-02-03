#pragma once

#include <cstdint>
#include <string>
#include <chrono>

enum class Side { BUY, SELL };

struct Order {
    uint64_t id;
    std::string symbol;
    Side side;
    int64_t price;  // Price in cents or smallest unit
    uint64_t quantity;
    std::chrono::high_resolution_clock::time_point timestamp;

    Order() = default;
    Order(uint64_t id, const std::string& symbol, Side side, int64_t price, uint64_t quantity)
        : id(id), symbol(symbol), side(side), price(price), quantity(quantity),
          timestamp(std::chrono::high_resolution_clock::now()) {}
};