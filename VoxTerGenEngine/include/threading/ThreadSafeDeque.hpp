#ifndef THREAD_SAFE_DEQUE_HPP
#define THREAD_SAFE_DEQUE_HPP

#include <thread>
#include <deque>
#include <stop_token>
#include <mutex>
#include <optional>

template <typename T>
class ThreadSafeDeque
{
private:
    std::deque<T> deque_;
    mutable std::mutex deque_mutex_;

public:
    void PushFront(T value)
    {
        std::lock_guard<std::mutex> lock(deque_mutex_);
        deque_.push_front(std::move(value));
    }

    void PushBack(T value)
    {
        std::lock_guard<std::mutex> lock(deque_mutex_);
        deque_.push_back(std::move(value));
    }

    std::optional<T> TryPopFront()
    {
        std::lock_guard<std::mutex> lock(deque_mutex_);

        if (deque_.empty())
        {
            return std::nullopt;
        }

        std::optional<T> ret = { std::move(deque_.front()) } ;
        deque_.pop_front();

        return ret;
    }

    std::optional<T> TryPopBack()
    {
        std::lock_guard<std::mutex> lock(deque_mutex_);

        if (deque_.empty())
        {
            return std::nullopt;
        }

        std::optional<T> ret = { std::move(deque_.back()) };
        deque_.pop_back();

        return ret;
    }

    bool Empty() const noexcept 
    {
        std::lock_guard<std::mutex> lock(deque_mutex_);
        return deque_.empty();
    }
    
};

#endif // THREAD_SAFE_DEQUE_HPP

