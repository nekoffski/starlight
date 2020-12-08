#pragma once

#include <vector>

#include "Timer.hpp"

namespace sl::async::timer {

class TimerEngine {
public:
    void update(float dtime) {
        for (auto& timer : m_timers) {
            if (timer->m_activated) {
                timer->m_timer -= dtime;
                timer->m_activated = false;
            }
        }
    }

    std::shared_ptr<Timer> createTimer(float sleepTime) {
        auto timer = std::make_shared<Timer>(sleepTime);
        m_timers.push_back(timer);

        return timer;
    }

private:
    std::vector<std::shared_ptr<Timer>> m_timers;
};
}
