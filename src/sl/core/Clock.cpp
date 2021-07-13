#include "Clock.h"

#include <ctime>
#include <iomanip>

using namespace std::literals::chrono_literals;

namespace sl::core {

float Clock::getDeltaTime() const {
    return m_deltaTime;
}

float Clock::toSeconds(const TimePoint& point) {
    return std::chrono::duration_cast<std::chrono::microseconds>(point.time_since_epoch()).count() / microsecondsInSecond;
}

void Clock::update() {
    auto now = this->now();
    toSeconds(now - m_previousNow);
    m_deltaTime = toSeconds(now - m_previousNow);
    m_previousNow = now;
}

float Clock::getFPS() const {
    return 1.0f / m_deltaTime;
}

std::string Clock::getTimeString(const std::string& format) const {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}

Clock::TimePoint Clock::now() const {
    return m_clock.now();
}

float Clock::nowAsFloat() const {
    return toSeconds(now());
}

}
