#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/OrderBook.hpp"
#include "../src/MatchingEngine.hpp"
#include "../src/RingBuffer.hpp"

TEST_CASE("OrderBook add and cancel", "[OrderBook]") {
    OrderBook ob;
    Order o1(1, "AAPL", Side::BUY, 10000, 100);
    ob.addOrder(o1);
    REQUIRE(ob.getBids().size() == 1);
    REQUIRE(ob.getBids().begin()->second.size() == 1);

    ob.cancelOrder(1);
    REQUIRE(ob.getBids().empty());
}

TEST_CASE("MatchingEngine match", "[MatchingEngine]") {
    RingBuffer<Order> queue(10);
    int tradeCount = 0;
    auto callback = [&tradeCount](const Trade&) { tradeCount++; };

    MatchingEngine me(queue, callback);

    Order buy(1, "AAPL", Side::BUY, 10000, 100);
    Order sell(2, "AAPL", Side::SELL, 10000, 50);

    me.addOrder(buy);
    me.addOrder(sell);

    // Wait a bit for async
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    REQUIRE(tradeCount == 1);
}