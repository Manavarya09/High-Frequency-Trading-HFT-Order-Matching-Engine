#pragma once

#include <chrono>
#include <iostream>

class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        std::cout << "Time: " << duration.count() << " us" << std::endl;
    }
private:
    std::chrono::high_resolution_clock::time_point start_;
};