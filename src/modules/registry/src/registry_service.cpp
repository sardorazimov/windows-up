#include "miransas/registry/registry_service.hpp"

#include "miransas/registry/registry_manager.hpp"

namespace miransas::registry
{
core::Result<std::string> WindowsRegistryService::read_value(std::string_view key, std::string_view value_name) const
{
    RegistryManager manager;
    return manager.ReadString(key, value_name);
}

core::Result<void> WindowsRegistryService::write_value(std::string_view key, std::string_view value_name, std::string_view value) const
{
    RegistryManager manager;
    return manager.WriteString(key, value_name, value);
}
} // namespace miransas::registry
