#ifndef DDS_THREADSAFEQUEUE_HPP
#define DDS_THREADSAFEQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

namespace dds {

/**
 * @brief ThreadSafeQueue provides a simple thread-safe FIFO queue for message passing.
 * Uses std::mutex and std::condition_variable for synchronization.
 * @tparam T The type of elements stored in the queue.
 */
template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    /**
     * @brief Push an item into the queue.
     * @param value The item to push
     */
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
        cond_.notify_one();
    }

    /**
     * @brief Try to pop an item from the queue (non-blocking).
     * @return std::optional<T> with value if available, std::nullopt otherwise
     */
    std::optional<T> try_pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return std::nullopt;
        T value = queue_.front();
        queue_.pop();
        return value;
    }

    /**
     * @brief Wait and pop an item from the queue (blocking).
     * @return The popped item
     */
    T wait_and_pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]{ return !queue_.empty(); });
        T value = queue_.front();
        queue_.pop();
        return value;
    }

    /**
     * @brief Check if the queue is empty.
     * @return true if empty, false otherwise
     */
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cond_;
};

} // namespace dds

#endif // DDS_THREADSAFEQUEUE_HPP 