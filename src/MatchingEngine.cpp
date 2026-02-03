#include "MatchingEngine.hpp"

void MatchingEngine::addOrder(const Order& order) {
    orderBook_.addOrder(order);
    match();
}

void MatchingEngine::cancelOrder(uint64_t orderId) {
    orderBook_.cancelOrder(orderId);
}

void MatchingEngine::match() {
    auto& bids = orderBook_.getBids();
    auto& asks = orderBook_.getAsks();

    while (!bids.empty() && !asks.empty() && bids.begin()->first >= asks.begin()->first) {
        auto& bidLevel = bids.begin()->second;
        auto& askLevel = asks.begin()->second;

        while (!bidLevel.empty() && !askLevel.empty()) {
            Order& bid = bidLevel.front();
            Order& ask = askLevel.front();

            uint64_t tradeQty = std::min(bid.quantity, ask.quantity);
            Trade trade{bid.id, ask.id, ask.price, tradeQty, std::chrono::high_resolution_clock::now()};
            tradeCallback_(trade);

            bid.quantity -= tradeQty;
            ask.quantity -= tradeQty;

            if (bid.quantity == 0) {
                bidLevel.pop_front();
            }
            if (ask.quantity == 0) {
                askLevel.pop_front();
            }
        }

        if (bidLevel.empty()) bids.erase(bids.begin());
        if (askLevel.empty()) asks.erase(asks.begin());
    }
}