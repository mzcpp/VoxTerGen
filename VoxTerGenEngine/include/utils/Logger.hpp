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

/**
 * @brief Logging levels for the Logger class.
 */
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
	/**
	 * @brief Initializes the logger.
	 *
	 * @param file_path Path to log file
	 * @param console_enabled flag determining whether console logging is enabled
	 * @param file_enabled flag determining whether file logging is enabled
	 */
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

	/**
	 * @brief Shuts down the logger and closes the file.
	 */
	static void ShutDown()
	{
		if (log_file_.is_open())
		{
			std::lock_guard<std::mutex> lock(log_mutex_);
			log_file_.flush();
			log_file_.close();
		}
	}

	/**
	 * @brief Enables or disables console logging.
	 * 
	 * @param e True to enable, false to disable
	 */
	static void EnableConsole(bool e)
	{
		std::lock_guard<std::mutex> lock(log_mutex_);
		console_enabled_ = e;
	}

	/**
	 * @brief Enables or disables file logging.
	 * 
	 * @param e True to enable, false to disable
	 */
	static void EnableFile(bool e)
	{
		std::lock_guard<std::mutex> lock(log_mutex_);
		file_enabled_ = e;
	}

	/**
	 * @brief Gets the current UTC time as a formatted string.
	 * 
	 * @return Formatted timestamp string
	 */
	static std::string GetCurrentDateTimeUTC()
	{
		using namespace std::chrono;
		auto now = floor<seconds>(system_clock::now());
		return std::format("[{:%Y-%m-%d %H:%M:%S}]", now);
	}

	/**
	 * @brief Gets the current local time as a formatted string.
	 * 
	 * @return Formatted timestamp string
	 */
	static std::string GetCurrentDateTimeLocal()
	{
		using namespace std::chrono;
		auto now = floor<seconds>(system_clock::now());
		zoned_time local{ current_zone(), now };
		return std::format("[{:%Y-%m-%d %H:%M:%S}]", local);
	}

	/**
	 * @brief Logs a message at the INFO level.
	 * 
	 * @tparam Args Format argument types
	 * @param fmt Format string
	 * @param args Arguments for formatting
	 */
	template <typename... Args>
	static void Log(std::format_string<Args...> fmt, Args&&... args)
	{
		Log(LogLevel::INFO, fmt, std::forward<Args>(args)...);
	}

	/**
	 * @brief Logs a message at a specified log level.
	 * 
	 * @tparam Args Format argument types
	 * @param level Log level
	 * @param fmt Format string
	 * @param args Arguments for formatting
	 */
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
	/**
	 * @brief Converts a LogLevel enum to its string representation.
	 * 
	 * @param level Log level
	 * 
	 * @return Corresponding string
	 */
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

#endif // LOGGER_HPP
