#pragma once

#include <unordered_map>

#include "Task.h"
#include "sl/core/Logger.h"
#include "sl/core/Macros.h"

namespace sl::task {

class TaskManager {
    SL_SINGLETON(TaskManager);

public:
    void processTasks();

    template <typename T, typename... Args>
    requires std::derived_from<T, Task> Task::Handle addPeriodicTask(Args&&... args) {
        auto task = std::make_unique<T>(std::forward<Args>(args)...);

        SL_DEBUG("Adding task to TaskManager: {}", task->getName());

        auto id = task->id;
        m_tasks[id] = std::move(task);

        return Task::Handle {
            m_tasks.at(id).get()
        };
    }

private:
    std::unordered_map<std::string, std::unique_ptr<Task>> m_tasks;
};
}

#define TASK_MANAGER() sl::task::TaskManager::instance()