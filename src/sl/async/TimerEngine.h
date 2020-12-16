#pragma once

#include <memory>
#include <vector>

#include "Timer.h"

namespace sl::async::detail {

class TimerEngine {
public:
    void update(float dtime);
    std::shared_ptr<Timer> createTimer(float sleepTime);

private:
    std::vector<std::shared_ptr<Timer>> m_timers;
};
}
