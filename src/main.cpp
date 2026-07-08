#include <iostream>

#include "miransas/ui/imgui_layer.hpp"

int main()
{
    try
    {
        miransas::ui::ImGuiLayer layer{"Miransas Control"};
        layer.run();
        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Application failed: " << ex.what() << '\n';
        return 1;
    }
}
