#include "OrderBook.hpp"

void OrderBook::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        bids_[order.price].push_back(order);
    } else {
        asks_[order.price].push_back(order);
    }
    orderLookup_[order.id] = {order.price, order.side};
}

void OrderBook::cancelOrder(uint64_t orderId) {
    auto it = orderLookup_.find(orderId);
    if (it == orderLookup_.end()) return;

    auto [price, side] = it->second;
    if (side == Side::BUY) {
        auto& level = bids_[price];
        level.erase(std::remove_if(level.begin(), level.end(),
            [orderId](const Order& o) { return o.id == orderId; }), level.end());
        if (level.empty()) bids_.erase(price);
    } else {
        auto& level = asks_[price];
        level.erase(std::remove_if(level.begin(), level.end(),
            [orderId](const Order& o) { return o.id == orderId; }), level.end());
        if (level.empty()) asks_.erase(price);
    }
    orderLookup_.erase(it);
}