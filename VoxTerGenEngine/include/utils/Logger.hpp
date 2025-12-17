#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <format>
#include <mutex>
#include <string_view>

enum class LogLevel
{
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

class Logger
{
private:
	static inline std::ofstream log_file_;
	static inline std::mutex log_mutex_;

	static inline bool console_enabled_ = false;
	static inline bool file_enabled_ = false;
	static inline bool initialized_ = false;

public:
	static void Init(const char* file_path, bool console_enabled, bool file_enabled)
	{
		std::lock_guard<std::mutex> lock(log_mutex_);

		if (log_file_.is_open())
		{
			log_file_.close();
		}

		console_enabled_ = console_enabled;
		file_enabled_ = file_enabled;
		
		try
		{
			log_file_.open(file_path, std::ios::out);
			file_enabled_ = log_file_.is_open();
		}
		catch (...)
		{
			file_enabled_ = false;
		}

		initialized_ = true;
	}

	static void ShutDown()
	{
		if (log_file_.is_open())
		{
			std::lock_guard<std::mutex> lock(log_mutex_);
			log_file_.flush();
			log_file_.close();
		}
	}

	static void EnableConsole(bool e)
	{
		std::lock_guard<std::mutex> lock(log_mutex_);
		console_enabled_ = e;
	}

	static void EnableFile(bool e)
	{
		std::lock_guard<std::mutex> lock(log_mutex_);
		file_enabled_ = e;
	}

	static std::string GetCurrentDateTimeUTC()
	{
		using namespace std::chrono;
		auto now = floor<seconds>(system_clock::now());
		return std::format("[{:%Y-%m-%d %H:%M:%S}]", now);
	}

	static std::string GetCurrentDateTimeLocal()
	{
		using namespace std::chrono;
		auto now = floor<seconds>(system_clock::now());
		zoned_time local{ current_zone(), now };
		return std::format("[{:%Y-%m-%d %H:%M:%S}]", local);
	}

	template <typename... Args>
	static void Log(std::format_string<Args...> fmt, Args&&... args)
	{
		Log(LogLevel::INFO, fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void Log(LogLevel level, std::format_string<Args...> fmt, Args&&... args)
	{
		std::lock_guard<std::mutex> lock(log_mutex_);

		if (!initialized_)
		{
			return;
		}

		const auto timestamp_utc = GetCurrentDateTimeUTC();
		const auto timestamp_local = GetCurrentDateTimeLocal();
		const auto log_level = std::format("{:<8}", LogLevelToString(level));

		const std::string message = std::format(fmt, std::forward<Args>(args)...);


		if (console_enabled_)
		{
			std::cout << std::format("{} {} {}\n", timestamp_local, log_level, message);

			if (level == LogLevel::CRITICAL || level == LogLevel::ERROR)
			{
				std::cout << std::flush;
			}
		}

		if (file_enabled_ && log_file_.is_open())
		{
			log_file_ << std::format("{} {} {}\n", timestamp_utc, log_level, message);
			
			if (level == LogLevel::CRITICAL || level == LogLevel::ERROR)
			{
				log_file_.flush();
			}
		}
	}

private:
	static std::string_view LogLevelToString(LogLevel level)
	{
		switch (level)
		{
			case LogLevel::DEBUG:
				return "DEBUG";
			case LogLevel::INFO:
				return "INFO";
			case LogLevel::WARNING:
				return "WARNING";
			case LogLevel::ERROR:
				return "ERROR";
			case LogLevel::CRITICAL:
				return "CRITICAL";
			default:
				return "UNKNOWN";
		}
	}
};

#endif

