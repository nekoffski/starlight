#pragma once

#include <limits>
#include <map>
#include <string>
#include <vector>

#include "timer/Timer.hpp"
#include "timer/TimerEngine.hpp"

namespace sl::async {

class AsyncEngine {
public:
    static void update(float dtime) {
        m_timerEngine.update(dtime);
    }

    static std::shared_ptr<timer::Timer> createTimer(float sleepTime) {
        return m_timerEngine.createTimer(sleepTime);
    }

private:
    inline static timer::TimerEngine m_timerEngine;
};
}
