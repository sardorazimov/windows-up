#include <iostream>

#include "miransas/registry/registry_manager.hpp"

int main()
{
    using namespace miransas::registry;

    RegistryManager manager;

    const auto key_path = std::string{"Software\\MiransasControlTest"};
    const auto value_name = std::string{"TestValue"};
    const auto value = std::string{"Hello Registry"};

#ifdef _WIN32
    auto write_result = manager.WriteString(key_path, value_name, value);
    if (!write_result)
    {
        std::cerr << write_result.error().to_string() << '\n';
        return 1;
    }

    auto read_result = manager.ReadString(key_path, value_name);
    if (!read_result || read_result.value() != value)
    {
        std::cerr << "Read-back mismatch" << '\n';
        return 1;
    }

    auto dword_result = manager.WriteDWORD(key_path, "TestDword", 42u);
    if (!dword_result)
    {
        std::cerr << dword_result.error().to_string() << '\n';
        return 1;
    }

    auto read_dword_result = manager.ReadDWORD(key_path, "TestDword");
    if (!read_dword_result || read_dword_result.value() != 42u)
    {
        std::cerr << "DWORD read-back mismatch" << '\n';
        return 1;
    }

    auto delete_value_result = manager.DeleteValue(key_path, value_name);
    if (!delete_value_result)
    {
        std::cerr << delete_value_result.error().to_string() << '\n';
        return 1;
    }

    auto delete_key_result = manager.DeleteKey(key_path);
    if (!delete_key_result)
    {
        std::cerr << delete_key_result.error().to_string() << '\n';
        return 1;
    }

    std::cout << "Registry module tests passed" << std::endl;
#else
    std::cout << "Registry module tests skipped on non-Windows host" << std::endl;
#endif
    return 0;
}
