#include "TaskManager.h"

namespace sl::task {

void TaskManager::processTasks() {
    std::vector<std::string> tasksToRemove;

    for (auto& [id, task] : m_tasks) {
        if (not task->isActive) {
            SL_DEBUG("Task {} not active, will remove", task->getName());
            tasksToRemove.push_back(id);
            continue;
        }

        if (task->shouldInvoke()) {
            SL_DEBUG("Invoking task: {}", task->getName());
            task->invoke();
        }
    }

    for (auto& id : tasksToRemove)
        m_tasks.erase(id);
}

}