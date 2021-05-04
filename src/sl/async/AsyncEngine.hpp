#pragma once

#include <future>
#include <memory>
#include <type_traits>

#include "ThreadPool.hpp"
#include "Timer.h"
#include "TimerEngine.h"
#include "sl/core/Macros.h"

namespace sl::async {

class AsyncEngine {
    SL_SINGLETON(AsyncEngine);

public:
    void init();
    void deinit();

    template <typename F, typename... Args>
    Future<typename std::result_of<F(Args...)>::type> callAsync(F&& func, Args&&... args) {
        return m_threadPool->callAsync<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
    }

    void parallelLoop(const int iterations, const std::function<void(const int)>& func);
    void update(float dtime);
    std::shared_ptr<Timer> createTimer(float sleepTime);

private:
    detail::TimerEngine m_timerEngine;
    std::unique_ptr<ThreadPool<>> m_threadPool = nullptr;
};
}

#define ASYNC_ENGINE() sl::async::AsyncEngine::instance()