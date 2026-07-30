#include "threading/ThreadPool.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <type_traits>
#include <vector>
#include <queue>
#include <concepts>
#include <memory>

ThreadPool::ThreadPool(std::size_t threads_n)
{
    for (std::size_t i = 0; i < threads_n; ++i)
    {
        workers_.emplace_back([this]() {
            for (;;)
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    condition_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });

                    if (stop_ && tasks_.empty())
                    {
                        return;
                    }

                    task = std::move(tasks_.front());
                    tasks_.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }

    condition_.notify_all();
}
