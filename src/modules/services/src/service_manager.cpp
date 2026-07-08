#include "miransas/services/service_manager.hpp"

namespace miransas::services
{
core::Result<void> WindowsServiceManager::disable_service(std::string_view)
{
    return std::unexpected(core::Error{"Service management is not implemented yet.", "not_implemented"});
}

core::Result<void> WindowsServiceManager::enable_service(std::string_view)
{
    return std::unexpected(core::Error{"Service management is not implemented yet.", "not_implemented"});
}
} // namespace miransas::services
