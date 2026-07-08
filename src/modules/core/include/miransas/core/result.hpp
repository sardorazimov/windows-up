#pragma once

#include <expected>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>

namespace miransas::core
{
class Error
{
public:
    Error() = default;

    Error(std::string message,
          std::string code = "unknown",
          std::string_view source = "unknown",
          std::source_location location = std::source_location::current())
        : message_(std::move(message))
        , code_(std::move(code))
        , source_(source)
        , location_(location)
    {
    }

    [[nodiscard]] const std::string& message() const noexcept { return message_; }
    [[nodiscard]] const std::string& code() const noexcept { return code_; }
    [[nodiscard]] const std::string& source() const noexcept { return source_; }

    [[nodiscard]] std::string to_string() const
    {
        return code_ + ": " + message_ + " [" + source_ + "]";
    }

private:
    std::string message_;
    std::string code_;
    std::string source_;
    std::source_location location_;
};

template <typename T>
using Result = std::expected<T, Error>;

using VoidResult = std::expected<void, Error>;
} // namespace miransas::core
