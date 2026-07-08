#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <string_view>

#include "miransas/core/result.hpp"

namespace miransas::logger
{
class ILogger
{
public:
    virtual ~ILogger() = default;

    virtual void info(std::string_view message) = 0;
    virtual void warning(std::string_view message) = 0;
    virtual void error(std::string_view message) = 0;
};

class Logger final : public ILogger
{
public:
    Logger() = default;

    void info(std::string_view message) override;
    void warning(std::string_view message) override;
    void error(std::string_view message) override;

private:
    std::mutex mutex_;
};

using LoggerPtr = std::unique_ptr<ILogger>;

[[nodiscard]] core::Result<LoggerPtr> create_logger();
} // namespace miransas::logger
