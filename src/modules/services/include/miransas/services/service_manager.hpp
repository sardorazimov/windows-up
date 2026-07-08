#pragma once

#include <string_view>

#include "miransas/core/result.hpp"

namespace miransas::services
{
class ServiceManager
{
public:
    virtual ~ServiceManager() = default;

    [[nodiscard]] virtual core::Result<void> disable_service(std::string_view service_name) = 0;
    [[nodiscard]] virtual core::Result<void> enable_service(std::string_view service_name) = 0;
};

class WindowsServiceManager final : public ServiceManager
{
public:
    [[nodiscard]] core::Result<void> disable_service(std::string_view service_name) override;
    [[nodiscard]] core::Result<void> enable_service(std::string_view service_name) override;
};
} // namespace miransas::services
