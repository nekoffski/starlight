#pragma once

#include "sl/core/ClockImpl.h"

#include <chrono>

#include "sl/core/Logger.h"

namespace sl::platform::clock {

using Clock = std::chrono::steady_clock;
using TimeUnit = std::chrono::microseconds;

class StdTimestamp : public core::Timestamp {
public:
    StdTimestamp();
    StdTimestamp(std::chrono::time_point<Clock> time);

    float value() override;
    float substract(std::shared_ptr<core::Timestamp> rhs) override;
    float substract(const StdTimestamp& rhs);

private:
    std::chrono::time_point<Clock> m_time;
};

class StdClockImpl : public core::ClockImpl {
public:
    std::string getTimeString(std::string_view format) override;
    float getDeltaTime() override;
    void update() override;
    std::shared_ptr<core::Timestamp> now() override;

private:
    float m_delta;
    StdTimestamp m_previous;
};
}
