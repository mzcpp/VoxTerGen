#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

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
#include <stop_token>

class ThreadPool
{
private:
    std::vector<std::jthread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
    int jobs_submitted_limit_ = 2048;

public:
    ThreadPool(std::size_t thread_count);

    ~ThreadPool();

    template <class Fn, class... Args>
    auto Enqueue(Fn&& fn, Args&&... args) -> std::future<typename std::invoke_result_t<Fn, Args...>>
    {
        using return_type = typename std::invoke_result_t<Fn, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));

        std::future<return_type> result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            if (stop_)
            {
                throw std::runtime_error("Enqueue on stopped ThreadPool!");
            }

            tasks_.emplace([task]() { (*task)(); });
        }

        condition_.notify_one();
        return result;
    }

    // Getters
    int JobsSubmittedLimit() const noexcept { return jobs_submitted_limit_; }

    // Setters
    void SetJobsSubmittedLimit(int jobs_submitted_limit) noexcept { jobs_submitted_limit_ = jobs_submitted_limit; }
};

#endif // THREAD_POOL_HPP

