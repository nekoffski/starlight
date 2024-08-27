

#include "Clock.hh"

using namespace std::literals::chrono_literals;

namespace sl {

Clock::Clock() : m_deltaTime(0.0f) {}

float Clock::getDeltaTime() const { return m_deltaTime; }

void Clock::update() {
    auto now        = m_clock.now();
    m_deltaTime     = kc::core::toSeconds(now - m_lastTimestamp);
    m_lastTimestamp = now;
}

float Clock::getFps() const { return 1.0f / m_deltaTime; }

}  // namespace sl
