#pragma once

#include "ClockImpl.h"

#include <chrono>

#include "starlight/core/log/Logger.h"

namespace sl::platform::time::impl {

using Clock = std::chrono::steady_clock;
using TimeUnit = std::chrono::microseconds;

static constexpr float DIVIDER = 1000.0f * 1000.0f;

class StdTimestamp : public ITimestamp {
public:
    StdTimestamp()
        : m_time(Clock::now()) {
    }

    StdTimestamp(std::chrono::time_point<Clock> time)
        : m_time(time) {
    }

    float value() override {
        return std::chrono::duration_cast<TimeUnit>(m_time.time_since_epoch()).count() / DIVIDER;
    }

    // could be possibly optimized
    float substract(const std::shared_ptr<ITimestamp>& rhs) override {
        auto ptr = std::static_pointer_cast<StdTimestamp>(rhs);
        return std::chrono::duration_cast<TimeUnit>(m_time - ptr->m_time).count() / DIVIDER;
    }

    float substract(const StdTimestamp& rhs) {
        return std::chrono::duration_cast<TimeUnit>(m_time - rhs.m_time).count() / DIVIDER;
    }

private:
    std::chrono::time_point<Clock> m_time;
};

class StdClockImpl : public IClockImpl {
public:
    float getDeltaTime() override {
        return m_delta;
    }

    void update() override {
        auto now = Clock::now();
        m_delta = StdTimestamp{ now }.substract(m_previous);
        m_previous = now;
    }

    std::shared_ptr<ITimestamp> now() override {
        return std::make_shared<StdTimestamp>();
    }

private:
    float m_delta;
    StdTimestamp m_previous;
};
}