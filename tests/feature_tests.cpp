#include <iostream>
#include <memory>
#include <string_view>

#include "miransas/core/feature.hpp"
#include "miransas/core/feature_manager.hpp"

namespace
{
class DemoFeature final : public miransas::core::Feature
{
public:
    std::string_view name() const noexcept override { return "DemoFeature"; }
    std::string_view Description() const noexcept override { return "A sample feature for tests"; }
    miransas::core::RiskLevel RiskLevel() const noexcept override { return miransas::core::RiskLevel::Low; }
    miransas::core::Result<bool> Check() const override { return true; }
    miransas::core::Result<void> Apply() override { return {}; }
    miransas::core::Result<void> Undo() override { return {}; }
};
} // namespace

int main()
{
    using namespace miransas::core;

    FeatureManager manager{[](std::string_view message) {
        std::cout << "[log] " << message << '\n';
    }};

    manager.register_feature(std::make_unique<DemoFeature>());

    auto apply_result = manager.ApplyFeature("DemoFeature");
    if (!apply_result)
    {
        std::cerr << apply_result.error().to_string() << '\n';
        return 1;
    }

    auto verify_result = manager.VerifyFeatureState("DemoFeature");
    if (!verify_result || !verify_result.value())
    {
        std::cerr << "Feature state verification failed" << '\n';
        return 1;
    }

    auto undo_result = manager.UndoFeature("DemoFeature");
    if (!undo_result)
    {
        std::cerr << undo_result.error().to_string() << '\n';
        return 1;
    }

    std::cout << "Feature system tests passed" << std::endl;
    return 0;
}
