#pragma once

#include <string_view>

#include "miransas/core/feature.hpp"

namespace miransas::cleaner
{
class CleanerFeature final : public core::Feature
{
public:
    std::string_view name() const noexcept override { return "Cleaner"; }
    bool initialize() override { return true; }
    bool shutdown() override { return true; }
};
} // namespace miransas::cleaner
