#pragma once

#include <memory>

#include "Timer.h"
#include "TimerEngine.h"

namespace sl::async {

class AsyncEngine {
public:
    static void update(float dtime) {
        m_timerEngine.update(dtime);
    }

    static std::shared_ptr<Timer> createTimer(float sleepTime) {
        return m_timerEngine.createTimer(sleepTime);
    }

private:
    inline static detail::TimerEngine m_timerEngine;
};
}
