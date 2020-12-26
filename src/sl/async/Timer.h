#pragma once

namespace sl::async {

namespace detail {
    class TimerEngine;
}

class Timer {
    friend class detail::TimerEngine;

public:
    explicit Timer(float sleepTime);

    void setSleepTime(float sleepTime);
    bool asyncSleep();

private:
    float m_sleepTime;
    float m_timer;
    bool m_activated;
};
}
