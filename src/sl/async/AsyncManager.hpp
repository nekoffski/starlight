#pragma once

#include <future>
#include <memory>
#include <type_traits>

#include <kc/async/ThreadPool.hpp>
#include <kc/core/Log.h>
#include <kc/core/Singleton.hpp>
#include <kc/core/Uuid.h>

#include "AsyncTask.h"
#include "PeriodicTask.h"
#include "Timer.h"

namespace sl::async {

class AsyncManager : public kc::core::Singleton<AsyncManager> {
    struct AsyncTaskSentinel {
        kc::async::Future<void> future;
        std::unique_ptr<AsyncTask> task;
    };

public:
    explicit AsyncManager();
    ~AsyncManager();

    template <typename T, typename... Args>
    requires std::derived_from<T, PeriodicTask> PeriodicTask::Handle addPeriodicTask(Args&&... args) {
        return addPeriodicTaskImpl(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template <typename F, typename... Args>
    kc::async::Future<typename std::result_of<F(Args...)>::type> callAsync(F&& func, Args&&... args) {
        return m_threadPool->callAsync<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    requires std::derived_from<T, AsyncTask> void callAsync(Args&&... args) {
        callAsyncImpl(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void update(float dtime);

    std::shared_ptr<Timer> createTimer(float sleepTime);

private:
    PeriodicTask::Handle addPeriodicTaskImpl(std::unique_ptr<PeriodicTask> periodicTask);
    void callAsyncImpl(std::unique_ptr<AsyncTask> asyncTask);

    void processPeriodicTasks();
    void processAsyncTasks();
    void processTimers(float dtime);

    std::unique_ptr<kc::async::ThreadPool> m_threadPool = nullptr;

    std::unordered_map<std::string, AsyncTaskSentinel> m_asyncTasks;
    std::unordered_map<std::string, std::unique_ptr<PeriodicTask>> m_periodicTasks;
    std::vector<std::shared_ptr<Timer>> m_timers;
};
}
