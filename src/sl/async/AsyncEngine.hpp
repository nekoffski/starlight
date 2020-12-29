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
    static void init() {
        m_threadPool = std::make_unique<ThreadPool<>>(std::thread::hardware_concurrency());
    }

    static void deinit() {
        m_threadPool->stop();
    }

    template <typename F, typename... Args>
    static Future<typename std::result_of<F(Args...)>::type> callAsync(F&& func, Args&&... args) {
        return m_threadPool->callAsync<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
    }

    static void parallelLoop(const int iterations, const std::function<void(const int)>& func) {
        auto threadCount = m_threadPool->getSize();
        auto batch = iterations / threadCount;

        std::vector<Future<void>> futures;
        futures.reserve(threadCount);

        for (int i = 0; i < threadCount; ++i) {
            int beg = i * batch;
            int end = (i + 1) * batch;

            if (i == threadCount - 1)
                end += iterations % batch;

            // clang-format off
            futures.emplace_back(
                m_threadPool->callAsync([func](int beg, int end) {
                    for (int i = beg; i < end; ++i)
                        func(i);
                }, beg, end));
            // clang-format on
        }

        wait(futures);
    }

    static void update(float dtime) {
        m_timerEngine.update(dtime);
    }

    static std::shared_ptr<Timer> createTimer(float sleepTime) {
        return m_timerEngine.createTimer(sleepTime);
    }

private:
    inline static detail::TimerEngine m_timerEngine;
    inline static std::unique_ptr<ThreadPool<>> m_threadPool = nullptr;
};
}
