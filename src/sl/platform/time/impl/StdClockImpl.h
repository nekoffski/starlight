#pragma once

#include "ClockImpl.h"

#include <chrono>

#include "sl/core/log/Logger.h"

namespace sl::platform::time::impl {

using Clock = std::chrono::steady_clock;
using TimeUnit = std::chrono::microseconds;

class StdTimestamp : public ITimestamp {
public:
    StdTimestamp();
    StdTimestamp(std::chrono::time_point<Clock> time);

    float value() override;
    float substract(const std::shared_ptr<ITimestamp>& rhs) override;
    float substract(const StdTimestamp& rhs);

private:
    std::chrono::time_point<Clock> m_time;
};

class StdClockImpl : public IClockImpl {
public:
    std::string getTimeString(std::string format) override;
    float getDeltaTime() override;
    void update() override;
    std::shared_ptr<ITimestamp> now() override;

private:
    float m_delta;
    StdTimestamp m_previous;
};
}