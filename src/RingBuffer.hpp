#pragma once

#include <atomic>
#include <vector>
#include <optional>

template<typename T>
class RingBuffer {
public:
    explicit RingBuffer(size_t capacity) : buffer_(capacity), capacity_(capacity) {}

    bool push(const T& item) {
        size_t head = head_.load(std::memory_order_relaxed);
        size_t tail = tail_.load(std::memory_order_acquire);
        size_t next_head = (head + 1) % capacity_;
        if (next_head == tail) return false;  // Full
        buffer_[head] = item;
        head_.store(next_head, std::memory_order_release);
        return true;
    }

    std::optional<T> pop() {
        size_t tail = tail_.load(std::memory_order_relaxed);
        size_t head = head_.load(std::memory_order_acquire);
        if (tail == head) return std::nullopt;  // Empty
        T item = buffer_[tail];
        tail_.store((tail + 1) % capacity_, std::memory_order_release);
        return item;
    }

private:
    std::vector<T> buffer_;
    size_t capacity_;
    std::atomic<size_t> head_{0};
    std::atomic<size_t> tail_{0};
};