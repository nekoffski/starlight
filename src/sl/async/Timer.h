#pragma once

#include "fwd.h"

namespace sl::async {

class Timer {
    friend class AsyncManager;

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
