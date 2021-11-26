#include "AsyncManager.hpp"

#include <kc/core/Log.h>

namespace sl::async {

AsyncManager::AsyncManager()
    : m_threadPool(std::make_unique<kc::async::ThreadPool>(4)) {
}

AsyncManager::~AsyncManager() {
    m_threadPool->stop();
}

void AsyncManager::update(float dtime) {
    processTimers(dtime);
    processPeriodicTasks();
    processAsyncTasks();
}

std::shared_ptr<Timer> AsyncManager::createTimer(float sleepTime) {
    auto timer = std::make_shared<Timer>(sleepTime);
    m_timers.push_back(timer);

    return timer;
}

PeriodicTask::Handle AsyncManager::addPeriodicTaskImpl(std::unique_ptr<PeriodicTask> periodicTask) {
    LOG_DEBUG("Adding periodic task to TaskManager: {}", periodicTask->getName());

    const auto& id = periodicTask->id;
    m_periodicTasks[id] = std::move(periodicTask);

    return PeriodicTask::Handle { m_periodicTasks.at(id).get() };
}

void AsyncManager::callAsyncImpl(std::unique_ptr<AsyncTask> asyncTask) {
    auto id = kc::core::generateUuid();

    LOG_INFO("Creating async task: {} with assigned id: {}", asyncTask->asString(), id);

    auto executeTask = [taskView = asyncTask.get()] {
        taskView->executeAsync();
    };

    m_asyncTasks.emplace(id,
        AsyncTaskSentinel { callAsync(executeTask), std::move(asyncTask) });
}

void AsyncManager::processPeriodicTasks() {
    std::vector<std::string> tasksToRemove;

    for (auto& [id, task] : m_periodicTasks) {
        if (not task->isActive) {
            LOG_DEBUG("Task {} not active, will remove", task->getName());
            tasksToRemove.push_back(id);
            continue;
        }

        if (task->shouldInvoke()) {
            LOG_DEBUG("Invoking task: {}", task->getName());
            task->invoke();
        }
    }

    for (auto& id : tasksToRemove)
        m_periodicTasks.erase(id);
}

void AsyncManager::processAsyncTasks() {
    std::vector<std::string> idsToRemove;

    for (auto& [id, taskSentinel] : m_asyncTasks) {
        auto& [future, task] = taskSentinel;

        if (future.isReady()) {
            LOG_INFO("Task {}/{} finished, finalizing.", task->asString(), id);

            task->finalize();
            idsToRemove.push_back(id);

            LOG_INFO("Task {}/{} finalized, will remove.", task->asString(), id);
        }
    }

    for (const auto& idToRemove : idsToRemove)
        m_asyncTasks.erase(idToRemove);
}

void AsyncManager::processTimers(float dtime) {
    for (auto& timer : m_timers) {
        if (timer->m_activated) {
            timer->m_timer -= dtime;
            timer->m_activated = false;
        }
    }
}

}
