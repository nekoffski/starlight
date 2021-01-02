#include "AsyncEngine.hpp"

namespace sl::async {

detail::TimerEngine AsyncEngine::m_timerEngine;
std::unique_ptr<ThreadPool<>> AsyncEngine::m_threadPool = nullptr;

void AsyncEngine::init() {
    m_threadPool = std::make_unique<ThreadPool<>>(std::thread::hardware_concurrency());
}

void AsyncEngine::deinit() {
    m_threadPool->stop();
}

void AsyncEngine::parallelLoop(const int iterations, const std::function<void(const int)>& func) {
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

void AsyncEngine::update(float dtime) {
    m_timerEngine.update(dtime);
}

std::shared_ptr<Timer> AsyncEngine::createTimer(float sleepTime) {
    return m_timerEngine.createTimer(sleepTime);
}
}
