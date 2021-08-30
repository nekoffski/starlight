#pragma once

#include <future>
#include <memory>
#include <type_traits>

#include <kc/core/Singleton.hpp>
#include <kc/core/Uuid.h>

#include "AsyncTask.h"
#include "TaskManager.h"
#include "ThreadPool.hpp"
#include "Timer.h"
#include "TimerEngine.h"
#include "sl/core/Logger.h"
#include "sl/core/Macros.h"

namespace sl::async {

class AsyncManager : public kc::core::Singleton<AsyncManager> {

    struct AsyncTaskSentinel {
        Future<void> future;
        std::unique_ptr<AsyncTask> task;
    };

public:
    using Ptr = std::unique_ptr<AsyncManager>;

    void start();
    void stop();

    template <typename T, typename... Args>
    requires std::derived_from<T, Task> Task::Handle addPeriodicTask(Args&&... args) {
        return m_taskManager.addPeriodicTask<T, Args...>(std::forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    Future<typename std::result_of<F(Args...)>::type> callAsync(F&& func, Args&&... args) {
        return m_threadPool->callAsync<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    requires std::derived_from<T, AsyncTask> void executeAsyncTask(Args&&... args) {
        auto task = std::make_unique<T>(std::forward<Args>(args)...);
        auto id = kc::core::generateUuid();

        SL_INFO("Creating async task: {} with assigned id: {}", task->asString(), id);

        m_asyncTasks.emplace(id,
            AsyncTaskSentinel { callAsync(&T::executeAsync, task.get()), std::move(task) });
    }

    void parallelLoop(const int iterations, const std::function<void(const int)>& func);
    void update(float dtime);
    std::shared_ptr<Timer> createTimer(float sleepTime);

private:
    TaskManager m_taskManager;
    detail::TimerEngine m_timerEngine;

    std::unique_ptr<ThreadPool<>> m_threadPool = nullptr;

    std::unordered_map<std::string, AsyncTaskSentinel> m_asyncTasks;
};
}
