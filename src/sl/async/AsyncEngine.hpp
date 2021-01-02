#pragma once

#include <future>
#include <memory>
#include <type_traits>

#include "ThreadPool.hpp"
#include "Timer.h"
#include "TimerEngine.h"

namespace sl::async {

class AsyncEngine {
public:
    static void init();
    static void deinit();

    template <typename F, typename... Args>
    static Future<typename std::result_of<F(Args...)>::type> callAsync(F&& func, Args&&... args) {
        return m_threadPool->callAsync<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
    }

    static void parallelLoop(const int iterations, const std::function<void(const int)>& func);
    static void update(float dtime);
    static std::shared_ptr<Timer> createTimer(float sleepTime);

private:
    static detail::TimerEngine m_timerEngine;
    static std::unique_ptr<ThreadPool<>> m_threadPool;
};
}
