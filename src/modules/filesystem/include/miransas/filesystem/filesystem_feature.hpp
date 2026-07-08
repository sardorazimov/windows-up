#pragma once

#include <string_view>

#include "miransas/core/feature.hpp"

namespace miransas::filesystem
{
class FilesystemFeature final : public core::Feature
{
public:
    std::string_view name() const noexcept override { return "Filesystem"; }
    bool initialize() override { return true; }
    bool shutdown() override { return true; }
};
} // namespace miransas::filesystem
