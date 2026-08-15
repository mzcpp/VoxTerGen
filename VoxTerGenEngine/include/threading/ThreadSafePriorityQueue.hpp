#ifndef THREAD_SAFE_PRIORITY_QUEUE_HPP
#define THREAD_SAFE_PRIORITY_QUEUE_HPP

#include <thread>
#include <queue>
#include <stop_token>
#include <mutex>
#include <optional>

template <typename T, typename Compare>
class ThreadSafePriorityQueue
{
private:
    std::priority_queue<T, std::vector<T>, Compare> priority_queue_;
    mutable std::mutex queue_mutex_;

public:
    void Push(T value)
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        priority_queue_.push(std::move(value));
    }

    std::optional<T> TryPop()
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);

        if (priority_queue_.empty())
        {
            return std::nullopt;
        }

        std::optional<T> ret = { std::move(priority_queue_.top()) };
        priority_queue_.pop();

        return ret;
    }

    bool Empty() const noexcept
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return priority_queue_.empty();
    }

};

#endif // THREAD_SAFE_PRIORITY_QUEUE_HPP