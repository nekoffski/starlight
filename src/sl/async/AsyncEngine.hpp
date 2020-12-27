#pragma once

#include <future>
#include <memory>
#include <type_traits>

#include "Timer.h"
#include "TimerEngine.h"

namespace sl::async {

class AsyncEngine {
public:
    template <typename T>
    struct Future {
        T getValue() {
            return future.get();
        }

        bool isReady() const {
            return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        }

        std::future<T> future;
    };

    static void update(float dtime) {
        m_timerEngine.update(dtime);
    }

    static std::shared_ptr<Timer> createTimer(float sleepTime) {
        return m_timerEngine.createTimer(sleepTime);
    }

    template <typename F, typename... Args>
    static auto callAsync(F&& callable, Args&&... args) {
        return Future{ std::async(std::launch::async, callable, std::forward<Args>(args)...) };
    }

private:
    inline static detail::TimerEngine m_timerEngine;
};
}
