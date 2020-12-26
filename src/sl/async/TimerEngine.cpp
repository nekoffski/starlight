#include "TimerEngine.h"

namespace sl::async::detail {

void TimerEngine::update(float dtime) {
    for (auto& timer : m_timers) {
        if (timer->m_activated) {
            timer->m_timer -= dtime;
            timer->m_activated = false;
        }
    }
}

std::shared_ptr<Timer> TimerEngine::createTimer(float sleepTime) {
    auto timer = std::make_shared<Timer>(sleepTime);
    m_timers.push_back(timer);

    return timer;
}
}
