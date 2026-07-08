#include "miransas/registry/registry_manager.hpp"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace miransas::registry
{
namespace
{
#ifdef _WIN32
using WideString = std::wstring;

WideString to_wide(std::string_view value)
{
    if (value.empty())
    {
        return {};
    }

    const int size = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (size <= 0)
    {
        throw std::system_error{static_cast<int>(GetLastError()), std::system_category(), "MultiByteToWideChar failed"};
    }

    WideString result(static_cast<std::size_t>(size), L'\0');
    const int written = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), result.data(), size);
    if (written <= 0)
    {
        throw std::system_error{static_cast<int>(GetLastError()), std::system_category(), "MultiByteToWideChar failed"};
    }

    result.resize(static_cast<std::size_t>(written));
    return result;
}

std::string from_wide(const WideString& value)
{
    if (value.empty())
    {
        return {};
    }

    const int size = WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0, nullptr, nullptr);
    if (size <= 0)
    {
        throw std::system_error{static_cast<int>(GetLastError()), std::system_category(), "WideCharToMultiByte failed"};
    }

    std::string result(static_cast<std::size_t>(size), '\0');
    const int written = WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), result.data(), size, nullptr, nullptr);
    if (written <= 0)
    {
        throw std::system_error{static_cast<int>(GetLastError()), std::system_category(), "WideCharToMultiByte failed"};
    }

    result.resize(static_cast<std::size_t>(written));
    return result;
}

class RegistryKeyHandle
{
public:
    explicit RegistryKeyHandle(HKEY key = nullptr, bool owned = true) noexcept
        : key_(key)
        , owned_(owned)
    {
    }

    ~RegistryKeyHandle()
    {
        if (owned_ && key_ != nullptr)
        {
            RegCloseKey(key_);
        }
    }

    RegistryKeyHandle(const RegistryKeyHandle&) = delete;
    RegistryKeyHandle& operator=(const RegistryKeyHandle&) = delete;

    RegistryKeyHandle(RegistryKeyHandle&& other) noexcept
        : key_(other.key_)
        , owned_(other.owned_)
    {
        other.key_ = nullptr;
        other.owned_ = false;
    }

    RegistryKeyHandle& operator=(RegistryKeyHandle&& other) noexcept
    {
        if (this != &other)
        {
            if (owned_ && key_ != nullptr)
            {
                RegCloseKey(key_);
            }
            key_ = other.key_;
            owned_ = other.owned_;
            other.key_ = nullptr;
            other.owned_ = false;
        }
        return *this;
    }

    [[nodiscard]] HKEY get() const noexcept { return key_; }
    [[nodiscard]] explicit operator bool() const noexcept { return key_ != nullptr; }

private:
    HKEY key_;
    bool owned_;
};

core::Error make_error(std::string_view operation, long error_code)
{
    std::ostringstream stream;
    stream << operation << " failed with error " << error_code;
    return core::Error{stream.str(), "win32_registry_error"};
}

core::Result<RegistryKeyHandle> open_key(HKEY root, std::wstring_view key_path, REGSAM access = KEY_READ)
{
    HKEY key = nullptr;
    const auto wide_path = std::wstring{key_path};
    const auto result = RegOpenKeyExW(root, wide_path.c_str(), 0, access, &key);
    if (result != ERROR_SUCCESS)
    {
        return std::unexpected(make_error("RegOpenKeyExW", result));
    }

    return RegistryKeyHandle{key, true};
}

core::Result<RegistryKeyHandle> create_key(HKEY root, std::wstring_view key_path)
{
    HKEY key = nullptr;
    const auto wide_path = std::wstring{key_path};
    DWORD disposition = 0;
    const auto result = RegCreateKeyExW(root, wide_path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &key, &disposition);
    if (result != ERROR_SUCCESS)
    {
        return std::unexpected(make_error("RegCreateKeyExW", result));
    }

    return RegistryKeyHandle{key, true};
}
#else
core::Result<void> unsupported_platform_error()
{
    return std::unexpected(core::Error{"Windows registry API is unavailable on this platform.", "unsupported_platform"});
}
#endif
} // namespace

core::Result<std::string> RegistryManager::ReadString(std::string_view key_path, std::string_view value_name) const
{
#ifdef _WIN32
    try
    {
        const auto wide_key = to_wide(key_path);
        const auto wide_value = to_wide(value_name);
        auto key_result = open_key(HKEY_CURRENT_USER, wide_key, KEY_READ);
        if (!key_result)
        {
            return std::unexpected(key_result.error());
        }

        RegistryKeyHandle key = std::move(key_result.value());
        DWORD type = 0;
        DWORD size = 0;
        const auto status = RegQueryValueExW(key.get(), wide_value.c_str(), nullptr, &type, nullptr, &size);
        if (status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegQueryValueExW", status));
        }

        if (type != REG_SZ && type != REG_EXPAND_SZ)
        {
            return std::unexpected(core::Error{"Registry value is not a string type.", "invalid_registry_type"});
        }

        std::vector<wchar_t> buffer(static_cast<std::size_t>(size / sizeof(wchar_t)) + 1, L'\0');
        const auto value_status = RegQueryValueExW(key.get(), wide_value.c_str(), nullptr, &type, reinterpret_cast<LPBYTE>(buffer.data()), &size);
        if (value_status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegQueryValueExW", value_status));
        }

        return from_wide(std::wstring{buffer.data()});
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "registry_exception"});
    }
#else
    return std::unexpected(core::Error{"Windows registry API is unavailable on this platform.", "unsupported_platform"});
#endif
}

core::Result<std::uint32_t> RegistryManager::ReadDWORD(std::string_view key_path, std::string_view value_name) const
{
#ifdef _WIN32
    try
    {
        const auto wide_key = to_wide(key_path);
        const auto wide_value = to_wide(value_name);
        auto key_result = open_key(HKEY_CURRENT_USER, wide_key, KEY_READ);
        if (!key_result)
        {
            return std::unexpected(key_result.error());
        }

        RegistryKeyHandle key = std::move(key_result.value());
        DWORD type = 0;
        DWORD size = sizeof(DWORD);
        DWORD value = 0;
        const auto status = RegQueryValueExW(key.get(), wide_value.c_str(), nullptr, &type, reinterpret_cast<LPBYTE>(&value), &size);
        if (status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegQueryValueExW", status));
        }

        if (type != REG_DWORD)
        {
            return std::unexpected(core::Error{"Registry value is not a DWORD type.", "invalid_registry_type"});
        }

        return value;
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "registry_exception"});
    }
#else
    return std::unexpected(core::Error{"Windows registry API is unavailable on this platform.", "unsupported_platform"});
#endif
}

core::Result<void> RegistryManager::WriteString(std::string_view key_path, std::string_view value_name, std::string_view value) const
{
#ifdef _WIN32
    try
    {
        const auto wide_key = to_wide(key_path);
        const auto wide_value = to_wide(value_name);
        const auto wide_data = to_wide(value);
        auto key_result = create_key(HKEY_CURRENT_USER, wide_key);
        if (!key_result)
        {
            return std::unexpected(key_result.error());
        }

        RegistryKeyHandle key = std::move(key_result.value());
        const auto status = RegSetValueExW(key.get(), wide_value.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(wide_data.c_str()), static_cast<DWORD>((wide_data.size() + 1) * sizeof(wchar_t)));
        if (status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegSetValueExW", status));
        }

        return {};
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "registry_exception"});
    }
#else
    return unsupported_platform_error();
#endif
}

core::Result<void> RegistryManager::WriteDWORD(std::string_view key_path, std::string_view value_name, std::uint32_t value) const
{
#ifdef _WIN32
    try
    {
        const auto wide_key = to_wide(key_path);
        const auto wide_value = to_wide(value_name);
        auto key_result = create_key(HKEY_CURRENT_USER, wide_key);
        if (!key_result)
        {
            return std::unexpected(key_result.error());
        }

        RegistryKeyHandle key = std::move(key_result.value());
        const auto status = RegSetValueExW(key.get(), wide_value.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(DWORD));
        if (status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegSetValueExW", status));
        }

        return {};
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "registry_exception"});
    }
#else
    return unsupported_platform_error();
#endif
}

core::Result<void> RegistryManager::DeleteValue(std::string_view key_path, std::string_view value_name) const
{
#ifdef _WIN32
    try
    {
        const auto wide_key = to_wide(key_path);
        const auto wide_value = to_wide(value_name);
        auto key_result = open_key(HKEY_CURRENT_USER, wide_key, KEY_SET_VALUE);
        if (!key_result)
        {
            return std::unexpected(key_result.error());
        }

        RegistryKeyHandle key = std::move(key_result.value());
        const auto status = RegDeleteValueW(key.get(), wide_value.c_str());
        if (status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegDeleteValueW", status));
        }

        return {};
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "registry_exception"});
    }
#else
    return unsupported_platform_error();
#endif
}

core::Result<void> RegistryManager::DeleteKey(std::string_view key_path) const
{
#ifdef _WIN32
    try
    {
        const auto wide_key = to_wide(key_path);
        const auto status = RegDeleteTreeW(HKEY_CURRENT_USER, wide_key.c_str());
        if (status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegDeleteTreeW", status));
        }

        return {};
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "registry_exception"});
    }
#else
    return unsupported_platform_error();
#endif
}

core::Result<void> RegistryManager::BackupKey(std::string_view key_path, std::string_view backup_file_path) const
{
#ifdef _WIN32
    try
    {
        const auto wide_key = to_wide(key_path);
        const auto wide_backup = to_wide(backup_file_path);
        auto key_result = open_key(HKEY_CURRENT_USER, wide_key, KEY_READ);
        if (!key_result)
        {
            return std::unexpected(key_result.error());
        }

        RegistryKeyHandle key = std::move(key_result.value());
        const auto status = RegSaveKeyExW(key.get(), wide_backup.c_str(), nullptr, 0);
        if (status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegSaveKeyExW", status));
        }

        return {};
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "registry_exception"});
    }
#else
    return unsupported_platform_error();
#endif
}

core::Result<void> RegistryManager::RestoreKey(std::string_view key_path, std::string_view backup_file_path) const
{
#ifdef _WIN32
    try
    {
        const auto wide_key = to_wide(key_path);
        const auto wide_backup = to_wide(backup_file_path);
        auto key_result = create_key(HKEY_CURRENT_USER, wide_key);
        if (!key_result)
        {
            return std::unexpected(key_result.error());
        }

        RegistryKeyHandle key = std::move(key_result.value());
        const auto status = RegRestoreKeyW(key.get(), wide_backup.c_str(), 0);
        if (status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegRestoreKeyW", status));
        }

        return {};
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "registry_exception"});
    }
#else
    return unsupported_platform_error();
#endif
}

core::Result<void> RegistryManager::ExportKey(std::string_view key_path, std::string_view export_file_path) const
{
#ifdef _WIN32
    try
    {
        const auto wide_key = to_wide(key_path);
        const auto wide_export = to_wide(export_file_path);
        auto key_result = open_key(HKEY_CURRENT_USER, wide_key, KEY_READ);
        if (!key_result)
        {
            return std::unexpected(key_result.error());
        }

        RegistryKeyHandle key = std::move(key_result.value());
        const auto status = RegSaveKeyExW(key.get(), wide_export.c_str(), nullptr, 0);
        if (status != ERROR_SUCCESS)
        {
            return std::unexpected(make_error("RegSaveKeyExW", status));
        }

        return {};
    }
    catch (const std::exception& ex)
    {
        return std::unexpected(core::Error{ex.what(), "registry_exception"});
    }
#else
    return unsupported_platform_error();
#endif
}
} // namespace miransas::registry
