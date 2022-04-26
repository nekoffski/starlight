#include "ClockManager.h"

#include <ctime>
#include <iomanip>

using namespace std::literals::chrono_literals;

namespace sl::core {

ClockManager::ClockManager() : m_deltaTime(0.0f) {}

float ClockManager::getDeltaTime() const { return m_deltaTime; }

void ClockManager::update() {
    auto now = m_clock.now();
    kc::core::toSeconds(now - m_previousNow);
    m_deltaTime   = kc::core::toSeconds(now - m_previousNow);
    m_previousNow = now;
}

float ClockManager::getFPS() const { return 1.0f / m_deltaTime; }

const ClockManager::Clock& ClockManager::getClock() { return m_clock; }

}  // namespace sl::core
