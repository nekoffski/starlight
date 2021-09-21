#pragma once

#include <chrono>
#include <memory>
#include <string>

#include <kc/core/Clock.h>
#include <kc/core/Singleton.hpp>

using namespace std::literals::chrono_literals;

namespace sl::core {

class ClockManager : public kc::core::Singleton<ClockManager> {
    using Clock = kc::core::Clock;

public:
    explicit ClockManager();

    float getFPS() const;
    float getDeltaTime() const;

    void update();

    const Clock& getClock();

private:
    Clock m_clock;
    Clock::TimePoint m_previousNow;

    float m_deltaTime;
};
}
