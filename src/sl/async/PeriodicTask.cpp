#include "PeriodicTask.h"

#include <kc/core/Uuid.h>

namespace sl::async {

PeriodicTask::Handle::Handle(PeriodicTask* task) : m_task(task) {}

void PeriodicTask::Handle::disable() {
    if (m_task != nullptr) m_task->isActive = false;
    m_task = nullptr;
}

PeriodicTask::PeriodicTask() : id(kc::core::generateUuid()), isActive(true) {}
}  // namespace sl::async