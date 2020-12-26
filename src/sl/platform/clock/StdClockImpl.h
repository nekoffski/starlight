#pragma once

#include "sl/core/ClockImpl.h"

#include <chrono>

#include "sl/core/Logger.h"

namespace sl::platform::clock {

using Clock = std::chrono::steady_clock;
using TimeUnit = std::chrono::microseconds;

class StdTimestamp : public core::ITimestamp {
public:
    StdTimestamp();
    StdTimestamp(std::chrono::time_point<Clock> time);

    float value() override;
    float substract(const std::shared_ptr<core::ITimestamp>& rhs) override;
    float substract(const StdTimestamp& rhs);

private:
    std::chrono::time_point<Clock> m_time;
};

class StdClockImpl : public core::IClockImpl {
public:
    std::string getTimeString(std::string format) override;
    float getDeltaTime() override;
    void update() override;
    std::shared_ptr<core::ITimestamp> now() override;

private:
    float m_delta;
    StdTimestamp m_previous;
};
}
