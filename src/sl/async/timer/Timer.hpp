#pragma once

namespace sl::async::timer {

class TimerEngine;

class Timer {
    friend class TimerEngine;

public:
    explicit Timer(float sleepTime)
        : m_sleepTime(sleepTime)
        , m_activated(false)
        , m_timer(m_sleepTime) {}

    void setSleepTime(float sleepTime) {
        m_sleepTime = sleepTime;

        if (m_timer > sleepTime)
            m_timer = sleepTime;
    }

    bool asyncSleep() {
        m_activated = true;
        if (m_timer <= 0) {
            m_timer = m_sleepTime;
            return false;
        }
        return true;
    }

private:
    float m_sleepTime;
    float m_timer;
    bool m_activated;
};
}
