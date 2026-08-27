#ifndef TIMER_HPP
#define TIMER_HPP

#include "utils/Logger.hpp"

#include <chrono>
#include <cstdint>
#include <limits>
#include <mutex>
#include <string>
#include <unordered_map>

class Timer
{
private:
	struct Statistics
	{
		std::uint64_t call_count_ = 0;
		std::uint64_t total_microseconds_ = 0;
		std::uint64_t min_microseconds_ = std::numeric_limits<std::uint64_t>::max();
		std::uint64_t max_microseconds_ = 0;
	};

	static void Record(const char* name, std::uint64_t microseconds)
	{
		std::lock_guard lock(mutex_);

		Statistics& stats = statistics_[name];

		++stats.call_count_;
		stats.total_microseconds_ += microseconds;

		if (microseconds < stats.min_microseconds_)
		{
			stats.min_microseconds_ = microseconds;
		}

		if (microseconds > stats.max_microseconds_)
		{
			stats.max_microseconds_ = microseconds;
		}
	}

	inline static std::unordered_map<std::string, Statistics> statistics_;
	inline static std::mutex mutex_;

public:
	class ScopedTimer
	{
	private:
		const char* name_;
		std::chrono::steady_clock::time_point start_;

	public:
		ScopedTimer(const char* name) : name_(name), start_(std::chrono::steady_clock::now())
		{
		}

		~ScopedTimer()
		{
			const auto end = std::chrono::steady_clock::now();
			const long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_).count();

			Timer::Record(name_, static_cast<std::uint64_t>(duration));
		}
	};

	static void PrintStatistics()
	{
		std::lock_guard lock(mutex_);

		for (const auto& [name, stats] : statistics_)
		{
			const double total_ms = stats.total_microseconds_ / 1000.0;
			const double average_ms = static_cast<double>(stats.total_microseconds_) / stats.call_count_ / 1000.0;
			const double min_ms = stats.min_microseconds_ / 1000.0;
			const double max_ms = stats.max_microseconds_ / 1000.0;

			Logger::Log(LogLevel::INFO,
				"[TIMER] {} | Calls: {} | Total: {:.3f} ms | Avg: {:.3f} ms | Min: {:.3f} ms | Max: {:.3f} ms",
				name,
				stats.call_count_,
				total_ms,
				average_ms,
				min_ms,
				max_ms
			);
		}
	}

	static void Reset()
	{
		std::lock_guard lock(mutex_);
		statistics_.clear();
	}
};

#endif // TIMER_HPP

#define TIMER_SCOPE(name) Timer::ScopedTimer timer_##__LINE__(name)