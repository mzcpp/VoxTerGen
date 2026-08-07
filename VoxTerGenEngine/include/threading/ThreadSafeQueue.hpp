#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <thread>
#include <queue>
#include <stop_token>
#include <mutex>
#include <optional>

template <typename T>
class ThreadSafeQueue
{
private:
    std::queue<T> queue_;
    mutable std::mutex queue_mutex_;

public:
    void Push(T value)
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        queue_.push(value);
    }

    std::optional<T> TryPop()
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);

        if (queue_.empty())
        {
            return std::nullopt;
        }

        std::optional<T> ret = { std::move(queue_.front()) } ;
        queue_.pop();

        return ret;
    }
    
};

#endif // THREAD_SAFE_QUEUE_HPP

