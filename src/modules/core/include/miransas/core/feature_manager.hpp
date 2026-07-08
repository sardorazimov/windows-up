#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "miransas/core/feature.hpp"
#include "miransas/core/result.hpp"

namespace miransas::core
{
class FeatureManager
{
public:
    using LoggerCallback = std::function<void(std::string_view)>;

    explicit FeatureManager(LoggerCallback logger = {}) noexcept;

    void register_feature(std::unique_ptr<Feature> feature);
    bool initialize_all();
    bool shutdown_all();
    void update_all();

    [[nodiscard]] core::Result<void> ApplyFeature(std::string_view name);
    [[nodiscard]] core::Result<void> UndoFeature(std::string_view name);
    [[nodiscard]] core::Result<bool> VerifyFeatureState(std::string_view name) const;

    [[nodiscard]] Feature* find_feature(std::string_view name) noexcept;
    [[nodiscard]] const Feature* find_feature(std::string_view name) const noexcept;
    [[nodiscard]] std::size_t feature_count() const noexcept;

private:
    struct ManagedFeature
    {
        std::unique_ptr<Feature> feature;
        bool is_applied{false};
    };

    void log_(std::string_view message) const;

    std::vector<ManagedFeature> features_;
    LoggerCallback logger_;
};
} // namespace miransas::core
