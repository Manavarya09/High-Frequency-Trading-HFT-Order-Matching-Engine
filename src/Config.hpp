#pragma once

#include <string>

struct Config {
    int port = 8080;
    size_t orderQueueSize = 1024;
    size_t tradeQueueSize = 1024;
    std::string logFile = "trades.log";
};