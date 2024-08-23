#pragma once

#include <kc/core/Clock.h>

#include <chrono>
#include <kc/core/Singleton.hpp>
#include <memory>
#include <string>

using namespace std::literals::chrono_literals;

namespace sl {

class TimeManager : public kc::core::Singleton<TimeManager> {
    using Clock = kc::core::Clock;

public:
    explicit TimeManager();

    float getFps() const;
    float getDeltaTime() const;

    void update();

    Clock* getClock();

private:
    Clock m_clock;
    Clock::TimePoint m_lastTimestamp;

    float m_deltaTime;
};

}  // namespace sl