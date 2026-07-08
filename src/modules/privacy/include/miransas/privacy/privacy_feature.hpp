#pragma once

#include <string_view>

#include "miransas/core/feature.hpp"

namespace miransas::privacy
{
class PrivacyFeature final : public core::Feature
{
public:
    std::string_view name() const noexcept override { return "Privacy"; }
    bool initialize() override { return true; }
    bool shutdown() override { return true; }
};
} // namespace miransas::privacy
