#include "Timer.h"

namespace sl::async {

Timer::Timer(float sleepTime)
    : m_sleepTime(sleepTime)
    , m_activated(false)
    , m_timer(m_sleepTime) {}

void Timer::setSleepTime(float sleepTime) {
    m_sleepTime = sleepTime;

    if (m_timer > sleepTime)
        m_timer = sleepTime;
}

bool Timer::asyncSleep() {
    m_activated = true;
    if (m_timer <= 0) {
        m_timer = m_sleepTime;
        return false;
    }
    return true;
}
}
