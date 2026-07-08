#include "miransas/ui/imgui_layer.hpp"

#include <iostream>
#include <memory>

#include "miransas/cleaner/cleaner_feature.hpp"
#include "miransas/core/feature_manager.hpp"
#include "miransas/diagnostics/diagnostics_feature.hpp"
#include "miransas/filesystem/filesystem_feature.hpp"
#include "miransas/packages/packages_feature.hpp"
#include "miransas/performance/performance_feature.hpp"
#include "miransas/privacy/privacy_feature.hpp"
#include "miransas/processes/processes_feature.hpp"
#include "miransas/restore/restore_feature.hpp"
#include "miransas/security/security_feature.hpp"

namespace miransas::ui
{
ImGuiLayer::ImGuiLayer(std::string_view app_name)
    : app_name_(app_name)
    , feature_manager_(std::make_unique<core::FeatureManager>())
{
    feature_manager_->register_feature(std::make_unique<privacy::PrivacyFeature>());
    feature_manager_->register_feature(std::make_unique<cleaner::CleanerFeature>());
    feature_manager_->register_feature(std::make_unique<performance::PerformanceFeature>());
    feature_manager_->register_feature(std::make_unique<diagnostics::DiagnosticsFeature>());
    feature_manager_->register_feature(std::make_unique<restore::RestoreFeature>());
    feature_manager_->register_feature(std::make_unique<security::SecurityFeature>());
    feature_manager_->register_feature(std::make_unique<processes::ProcessesFeature>());
    feature_manager_->register_feature(std::make_unique<packages::PackagesFeature>());
    feature_manager_->register_feature(std::make_unique<filesystem::FilesystemFeature>());
}

ImGuiLayer::~ImGuiLayer() = default;

void ImGuiLayer::run()
{
    std::cout << "Miransas Control initialized with " << feature_manager_->feature_count() << " feature modules.\n";
    std::cout << "UI and business logic are separated; feature implementations are scaffolded for future expansion.\n";
}
} // namespace miransas::ui
