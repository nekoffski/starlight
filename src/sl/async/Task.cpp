#include "Task.h"

#include <kc/core/Uuid.h>

namespace sl::async {

Task::Handle::Handle(Task* task)
    : m_task(task) {
}

void Task::Handle::disable() {
    if (m_task != nullptr)
        m_task->isActive = false;
    m_task = nullptr;
}

Task::Task()
    : id(kc::core::generateUuid()) {
}
}