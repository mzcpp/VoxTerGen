#include "threading/ThreadPool.hpp"

#include "utils/Logger.hpp"

#include <thread>
#include <mutex>
#include <cassert>
#include <condition_variable>
#include <future>
#include <functional>
#include <type_traits>
#include <vector>
#include <queue>
#include <concepts>
#include <memory>
#include <stop_token>

ThreadPool::ThreadPool(std::size_t thread_count)
{
    workers_.reserve(thread_count);

    for (std::size_t i = 0; i < thread_count; ++i)
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

                try
                {
                    task();
                }
                catch (const std::future_error& e)
                {
                    Logger::Log(
                        "ThreadPool packaged_task error: {} (code {})",
                        e.what(),
                        e.code().value()
                    );

                    assert(false);
                }
                catch (const std::exception& e)
                {
                    Logger::Log("ThreadPool task threw an exception: {}", e.what());
                }
                catch (...)
                {
                    Logger::Log("ThreadPool task threw an unknown exception.");
                }
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

    // Ensure all workers have stopped before destroying shared thread-pool state.
    workers_.clear();
}
