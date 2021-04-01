#include "StdClockImpl.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace sl::platform::core {

using Clock = std::chrono::steady_clock;
using TimeUnit = std::chrono::microseconds;

static constexpr float divider = 1000.0f * 1000.0f;

StdTimestamp::StdTimestamp()
    : m_time(Clock::now()) {
}

StdTimestamp::StdTimestamp(std::chrono::time_point<Clock> time)
    : m_time(time) {
}

float StdTimestamp::value() {
    return std::chrono::duration_cast<TimeUnit>(m_time.time_since_epoch()).count() / divider;
}

// could be possibly optimized
float StdTimestamp::substract(std::shared_ptr<Timestamp> rhs) {
    auto ptr = std::static_pointer_cast<StdTimestamp>(rhs);
    return std::chrono::duration_cast<TimeUnit>(m_time - ptr->m_time).count() / divider;
}

float StdTimestamp::substract(const StdTimestamp& rhs) {
    return std::chrono::duration_cast<TimeUnit>(m_time - rhs.m_time).count() / divider;
}

std::string StdClockImpl::getTimeString(const std::string& format) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}

float StdClockImpl::getDeltaTime() {
    return m_delta;
}

void StdClockImpl::update() {
    auto now = Clock::now();
    m_delta = StdTimestamp { now }.substract(m_previous);
    m_previous = now;
}

std::shared_ptr<sl::core::Timestamp> StdClockImpl::now() {
    return std::make_shared<StdTimestamp>();
}
}
