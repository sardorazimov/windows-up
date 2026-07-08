#include "miransas/core/windows_privilege.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace miransas::core
{
bool WindowsPrivilegeDetector::is_elevated()
{
#ifdef _WIN32
    BOOL elevated = FALSE;
    HANDLE token = nullptr;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
    {
        return false;
    }

    if (token != nullptr)
    {
        TOKEN_ELEVATION elevation{};
        DWORD size = 0;
        if (GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size))
        {
            elevated = elevation.TokenIsElevated;
        }

        CloseHandle(token);
    }

    return elevated != FALSE;
#else
    return false;
#endif
}

std::string WindowsPrivilegeDetector::privilege_status()
{
    return is_elevated() ? "Administrator privileges available" : "Standard user context";
}

core::Result<void> WindowsPrivilegeDetector::ensure_elevation(std::string_view operation_name)
{
    if (is_elevated())
    {
        return {};
    }

    return std::unexpected(core::Error{std::string{"Elevation required for "} + std::string{operation_name}, "elevation_required"});
}
} // namespace miransas::core
