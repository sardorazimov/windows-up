#pragma once

#include <string_view>

#include "miransas/core/result.hpp"

namespace miransas::scheduled_tasks
{
class ScheduledTaskManager
{
public:
    virtual ~ScheduledTaskManager() = default;

    [[nodiscard]] virtual core::Result<void> disable_task(std::string_view task_name) = 0;
    [[nodiscard]] virtual core::Result<void> enable_task(std::string_view task_name) = 0;
};

class WindowsScheduledTaskManager final : public ScheduledTaskManager
{
public:
    [[nodiscard]] core::Result<void> disable_task(std::string_view task_name) override;
    [[nodiscard]] core::Result<void> enable_task(std::string_view task_name) override;
};
} // namespace miransas::scheduled_tasks
