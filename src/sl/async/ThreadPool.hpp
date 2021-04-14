#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <future>
#include <thread>
#include <vector>

#include "Future.h"
#include "sl/core/AtomicQueue.hpp"

namespace sl::async {

template <class Thread = std::jthread>
class ThreadPool {
    using Job = std::function<void()>;

public:
    explicit ThreadPool(std::size_t size = 4)
        : m_active(true)
        , m_size(size) {
        m_threads.reserve(size);
        for (int i = 0; i < size; ++i)
            m_threads.emplace_back(Thread { &ThreadPool::threadWorker, this });
    }

    std::size_t getSize() const {
        return m_size;
    }

    template <typename F, typename... Args>
    Future<typename std::result_of<F(Args...)>::type> callAsync(F&& func, Args&&... args) {
        using ReturnType = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...));
        auto res = task->get_future();

        m_jobs.enqueue([task]() { (*task)(); });

        return Future<ReturnType>(std::move(res));
    }

    void stop() {
        m_active = false;
    }

private:
    void threadWorker() {
        using namespace std::chrono_literals;

        while (m_active) {
            auto job = m_jobs.dequeue();

            if (not job.has_value()) {
                // std::this_thread::yield();
                std::this_thread::sleep_for(100ms);
                continue;
            }
            (*job)();
        }
    }

    core::AtomicQueue<Job> m_jobs;
    std::vector<Thread> m_threads;
    std::atomic_bool m_active;
    std::size_t m_size;
};
}
