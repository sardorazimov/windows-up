#include "miransas/core/feature_manager.hpp"

#include <algorithm>
#include <iostream>
#include <utility>

namespace miransas::core
{
namespace
{
std::string build_missing_feature_message(std::string_view name)
{
    return std::string{"Feature not found: "} + std::string{name};
}
} // namespace

FeatureManager::FeatureManager(LoggerCallback logger) noexcept
    : logger_(std::move(logger))
{
}

void FeatureManager::register_feature(std::unique_ptr<Feature> feature)
{
    if (feature == nullptr)
    {
        log_("Attempted to register a null feature");
        return;
    }

    if (find_feature(feature->Name()) != nullptr)
    {
        log_(std::string{"Feature already registered: "} + std::string{feature->Name()});
        return;
    }

    features_.push_back(ManagedFeature{std::move(feature), false});
    log_(std::string{"Registered feature: "} + std::string{features_.back().feature->Name()});
}

bool FeatureManager::initialize_all()
{
    for (auto& managed : features_)
    {
        if (managed.feature == nullptr || !managed.feature->initialize())
        {
            return false;
        }
    }

    return true;
}

bool FeatureManager::shutdown_all()
{
    for (auto it = features_.rbegin(); it != features_.rend(); ++it)
    {
        if (it->feature != nullptr && !it->feature->shutdown())
        {
            return false;
        }
    }

    return true;
}

void FeatureManager::update_all()
{
    for (auto& managed : features_)
    {
        if (managed.feature != nullptr)
        {
            managed.feature->update();
        }
    }
}

core::Result<void> FeatureManager::ApplyFeature(std::string_view name)
{
    auto managed = std::find_if(features_.begin(), features_.end(), [&](const auto& item) {
        return item.feature != nullptr && item.feature->Name() == name;
    });

    if (managed == features_.end() || managed->feature == nullptr)
    {
        return std::unexpected(core::Error{build_missing_feature_message(name), "feature_not_found"});
    }

    log_(std::string{"Applying feature: "} + std::string{name});

    auto check_result = managed->feature->Check();
    if (!check_result)
    {
        log_(std::string{"Check failed for feature: "} + std::string{name});
        return std::unexpected(check_result.error());
    }

    if (!check_result.value())
    {
        return std::unexpected(core::Error{std::string{"Feature check failed: "} + std::string{name}, "feature_check_failed"});
    }

    auto apply_result = managed->feature->Apply();
    if (!apply_result)
    {
        log_(std::string{"Apply failed for feature: "} + std::string{name});
        return std::unexpected(apply_result.error());
    }

    managed->is_applied = true;
    log_(std::string{"Applied feature successfully: "} + std::string{name});
    return {};
}

core::Result<void> FeatureManager::UndoFeature(std::string_view name)
{
    auto managed = std::find_if(features_.begin(), features_.end(), [&](const auto& item) {
        return item.feature != nullptr && item.feature->Name() == name;
    });

    if (managed == features_.end() || managed->feature == nullptr)
    {
        return std::unexpected(core::Error{build_missing_feature_message(name), "feature_not_found"});
    }

    log_(std::string{"Undoing feature: "} + std::string{name});

    if (!managed->is_applied)
    {
        log_(std::string{"Feature already inactive: "} + std::string{name});
        return {};
    }

    auto undo_result = managed->feature->Undo();
    if (!undo_result)
    {
        log_(std::string{"Undo failed for feature: "} + std::string{name});
        return std::unexpected(undo_result.error());
    }

    managed->is_applied = false;
    log_(std::string{"Undid feature: "} + std::string{name});
    return {};
}

core::Result<bool> FeatureManager::VerifyFeatureState(std::string_view name) const
{
    const auto managed = std::find_if(features_.begin(), features_.end(), [&](const auto& item) {
        return item.feature != nullptr && item.feature->Name() == name;
    });

    if (managed == features_.end() || managed->feature == nullptr)
    {
        return std::unexpected(core::Error{build_missing_feature_message(name), "feature_not_found"});
    }

    log_(std::string{"Verifying feature state: "} + std::string{name});

    auto check_result = managed->feature->Check();
    if (!check_result)
    {
        return std::unexpected(check_result.error());
    }

    const bool is_active = managed->is_applied && check_result.value();
    return is_active;
}

Feature* FeatureManager::find_feature(std::string_view name) noexcept
{
    const auto it = std::find_if(features_.begin(), features_.end(), [&](const auto& managed) {
        return managed.feature != nullptr && managed.feature->Name() == name;
    });

    return it != features_.end() ? it->feature.get() : nullptr;
}

const Feature* FeatureManager::find_feature(std::string_view name) const noexcept
{
    const auto it = std::find_if(features_.begin(), features_.end(), [&](const auto& managed) {
        return managed.feature != nullptr && managed.feature->Name() == name;
    });

    return it != features_.end() ? it->feature.get() : nullptr;
}

std::size_t FeatureManager::feature_count() const noexcept
{
    return features_.size();
}

void FeatureManager::log_(std::string_view message) const
{
    if (logger_)
    {
        logger_(message);
        return;
    }

    std::cout << "[feature] " << message << '\n';
}
} // namespace miransas::core
