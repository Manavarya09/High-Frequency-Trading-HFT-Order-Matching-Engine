# High-Frequency-Trading-HFT-Order-Matching-Engine

A simplified High-Frequency Trading (HFT) Order Matching Engine implemented in C++.

## Architecture

- **Gateway**: Parses FIX-like messages from TCP clients into Order objects.
- **Matching Engine**: Processes orders using Price-Time Priority (FIFO), matches bids >= asks.
- **Logger**: Asynchronously logs trades to disk.
- **Ring Buffer**: Lock-free queue for inter-thread communication.

## Components

- `Order`: Represents a buy/sell order.
- `OrderBook`: Maintains bid/ask levels using std::map and std::deque.
- `MatchingEngine`: Core matching logic.
- `RingBuffer`: Lock-free SPSC queue.
- `Gateway`: Message parser.
- `Logger`: Async trade logger.
- `TCPServer`: TCP server for client connections.

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Run

```bash
./build/hft_engine
```

Send orders via TCP on port 8080, format: `ORDER|id|symbol|side|price|quantity`

Example: `ORDER|1|AAPL|B|10000|100`

Trades logged to `trades.log`.

## Latency Optimizations

- Lock-free Ring Buffer for order queue.
- Minimal heap allocations in hot path.
- Separate threads for matching and logging.