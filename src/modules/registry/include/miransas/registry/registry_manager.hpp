#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "miransas/core/result.hpp"

namespace miransas::registry
{
class RegistryManager
{
public:
    RegistryManager() = default;

    [[nodiscard]] core::Result<std::string> ReadString(std::string_view key_path, std::string_view value_name) const;
    [[nodiscard]] core::Result<std::uint32_t> ReadDWORD(std::string_view key_path, std::string_view value_name) const;

    [[nodiscard]] core::Result<void> WriteString(std::string_view key_path, std::string_view value_name, std::string_view value) const;
    [[nodiscard]] core::Result<void> WriteDWORD(std::string_view key_path, std::string_view value_name, std::uint32_t value) const;

    [[nodiscard]] core::Result<void> DeleteValue(std::string_view key_path, std::string_view value_name) const;
    [[nodiscard]] core::Result<void> DeleteKey(std::string_view key_path) const;

    [[nodiscard]] core::Result<void> BackupKey(std::string_view key_path, std::string_view backup_file_path) const;
    [[nodiscard]] core::Result<void> RestoreKey(std::string_view key_path, std::string_view backup_file_path) const;
    [[nodiscard]] core::Result<void> ExportKey(std::string_view key_path, std::string_view export_file_path) const;
};
} // namespace miransas::registry
