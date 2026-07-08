#pragma once

#include <string_view>

#include "miransas/core/result.hpp"

namespace miransas::core
{
enum class RiskLevel
{
    Low,
    Medium,
    High
};

class Feature
{
public:
    virtual ~Feature() = default;

    virtual std::string_view Name() const noexcept { return name(); }
    virtual std::string_view Description() const noexcept { return {}; }
    virtual RiskLevel RiskLevel() const noexcept { return RiskLevel::Medium; }
    virtual core::Result<bool> Check() const { return true; }
    virtual core::Result<void> Apply() { return {}; }
    virtual core::Result<void> Undo() { return {}; }

    virtual std::string_view name() const noexcept = 0;
    virtual bool initialize() { return true; }
    virtual bool shutdown() { return true; }
    virtual void update() {}
};
} // namespace miransas::core
