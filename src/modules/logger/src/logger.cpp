#include "miransas/logger/logger.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>

namespace miransas::logger
{
namespace
{
std::string format_timestamp()
{
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::tm local_time{};
#ifdef _WIN32
    localtime_s(&local_time, &time);
#else
    localtime_r(&time, &local_time);
#endif

    std::ostringstream stream;
    stream << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    return stream.str();
}
} // namespace

void Logger::info(std::string_view message)
{
    std::lock_guard lock{mutex_};
    std::cout << "[INFO] " << format_timestamp() << " " << message << '\n';
}

void Logger::warning(std::string_view message)
{
    std::lock_guard lock{mutex_};
    std::cout << "[WARN] " << format_timestamp() << " " << message << '\n';
}

void Logger::error(std::string_view message)
{
    std::lock_guard lock{mutex_};
    std::cout << "[ERROR] " << format_timestamp() << " " << message << '\n';
}

core::Result<LoggerPtr> create_logger()
{
    try
    {
        return LoggerPtr{std::make_unique<Logger>()};
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "logger_creation_failed"});
    }
}
} // namespace miransas::logger
