#pragma once

#include <memory>

#include "miransas/core/feature_manager.hpp"

namespace miransas::ui
{
class ImGuiLayer
{
public:
    explicit ImGuiLayer(std::string_view app_name);
    ~ImGuiLayer();

    ImGuiLayer(const ImGuiLayer&) = delete;
    ImGuiLayer& operator=(const ImGuiLayer&) = delete;
    ImGuiLayer(ImGuiLayer&&) = delete;
    ImGuiLayer& operator=(ImGuiLayer&&) = delete;

    void run();

private:
    std::string app_name_;
    std::unique_ptr<core::FeatureManager> feature_manager_;
};
} // namespace miransas::ui
