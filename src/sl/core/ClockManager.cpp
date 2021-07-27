#include "ClockManager.h"

#include <ctime>
#include <iomanip>

using namespace std::literals::chrono_literals;

namespace sl::core {

float ClockManager::getDeltaTime() const {
    return m_deltaTime;
}

float ClockManager::toSeconds(const TimePoint& point) {
    return std::chrono::duration_cast<std::chrono::microseconds>(point.time_since_epoch()).count() / microsecondsInSecond;
}

void ClockManager::update() {
    auto now = this->now();
    toSeconds(now - m_previousNow);
    m_deltaTime = toSeconds(now - m_previousNow);
    m_previousNow = now;
}

float ClockManager::getFPS() const {
    return 1.0f / m_deltaTime;
}

std::string ClockManager::getTimeString(const std::string& format) const {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, format.c_str());
    return oss.str();
}

ClockManager::TimePoint ClockManager::now() const {
    return m_clock.now();
}

float ClockManager::nowAsFloat() const {
    return toSeconds(now());
}

}
