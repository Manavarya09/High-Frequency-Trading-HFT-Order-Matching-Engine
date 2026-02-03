# High-Frequency-Trading-HFT-Order-Matching-Engine

A comprehensive High-Frequency Trading (HFT) Order Matching Engine implemented in modern C++.

## Features

- **Price-Time Priority Matching**: FIFO within price levels.
- **Lock-Free Communication**: Custom RingBuffer for inter-thread messaging.
- **Asynchronous Logging**: Non-blocking trade logging to disk.
- **TCP Server**: Accepts FIX-like order messages.
- **Client Simulator**: Test client for sending orders.
- **Benchmarking**: Performance measurement tools.
- **Configurable**: Runtime configuration via Config struct.

## Architecture

- **Gateway**: Parses TCP messages into Orders, queues via RingBuffer.
- **MatchingEngine**: Dedicated thread processes orders, matches bids >= asks.
- **Logger**: Async thread logs trades.
- **TCPServer**: Handles client connections.

## Build

```bash
mkdir build
cd build
cmake ..
make
```

Executables: `hft_engine`, `hft_client`, `hft_benchmark`

## Run

Server:
```bash
./build/hft_engine
```

Client:
```bash
./build/hft_client
```

Benchmark:
```bash
./build/hft_benchmark
```

## Message Format

`ORDER|id|symbol|side|price|quantity`

Side: B (Buy), S (Sell)

## Latency Optimizations

- Lock-free SPSC RingBuffer.
- Minimal allocations in hot path.
- Separate threads for I/O, matching, logging.
- High-resolution timing.

## Project Structure

- `src/`: Core engine code.
- `client/`: Test client.
- `benchmark/`: Performance tests.
- `tests/`: Unit tests (placeholder).