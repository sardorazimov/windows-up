#pragma once

#include <string>
#include <string_view>

#include "miransas/core/result.hpp"

namespace miransas::registry
{
class RegistryService
{
public:
    virtual ~RegistryService() = default;

    [[nodiscard]] virtual core::Result<std::string> read_value(std::string_view key, std::string_view value_name) const = 0;
    [[nodiscard]] virtual core::Result<void> write_value(std::string_view key, std::string_view value_name, std::string_view value) const = 0;
};

class WindowsRegistryService final : public RegistryService
{
public:
    [[nodiscard]] core::Result<std::string> read_value(std::string_view key, std::string_view value_name) const override;
    [[nodiscard]] core::Result<void> write_value(std::string_view key, std::string_view value_name, std::string_view value) const override;
};
} // namespace miransas::registry
