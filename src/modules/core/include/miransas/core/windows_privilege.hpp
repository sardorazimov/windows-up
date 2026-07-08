#pragma once

#include <string>
#include <string_view>

#include "miransas/core/result.hpp"

namespace miransas::core
{
class WindowsPrivilegeDetector
{
public:
    [[nodiscard]] static bool is_elevated();
    [[nodiscard]] static std::string privilege_status();
    [[nodiscard]] static core::Result<void> ensure_elevation(std::string_view operation_name);
};
} // namespace miransas::core
