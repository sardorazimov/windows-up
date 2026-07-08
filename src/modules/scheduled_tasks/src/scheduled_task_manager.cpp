#include "miransas/scheduled_tasks/scheduled_task_manager.hpp"

namespace miransas::scheduled_tasks
{
core::Result<void> WindowsScheduledTaskManager::disable_task(std::string_view)
{
    return std::unexpected(core::Error{"Scheduled task management is not implemented yet.", "not_implemented"});
}

core::Result<void> WindowsScheduledTaskManager::enable_task(std::string_view)
{
    return std::unexpected(core::Error{"Scheduled task management is not implemented yet.", "not_implemented"});
}
} // namespace miransas::scheduled_tasks
