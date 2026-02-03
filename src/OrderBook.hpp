#pragma once

#include "Order.hpp"
#include <map>
#include <deque>
#include <unordered_map>
#include <functional>

class OrderBook {
public:
    using PriceLevel = std::deque<Order>;
    using BidBook = std::map<int64_t, PriceLevel, std::greater<int64_t>>;  // Descending prices for bids
    using AskBook = std::map<int64_t, PriceLevel>;  // Ascending prices for asks

    void addOrder(const Order& order);
    void cancelOrder(uint64_t orderId);
    BidBook& getBids() { return bids_; }
    AskBook& getAsks() { return asks_; }

private:
    BidBook bids_;
    AskBook asks_;
    std::unordered_map<uint64_t, std::pair<int64_t, Side>> orderLookup_;  // orderId -> (price, side)
};