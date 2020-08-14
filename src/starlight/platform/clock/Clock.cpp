#include "Clock.h"

#include <GLFW/glfw3.h>

static constexpr float DIVIDER = 1.0f;

namespace sl::platform::clock {

float Clock::Timestamp::getTime() const {
    return m_ticksTimestamp / DIVIDER;
}

Clock::Timestamp::Timestamp(float ts)
    : m_ticksTimestamp(ts) {
}

Clock::Timestamp Clock::now() {
    return Timestamp(m_ticker->getTicks());
}

void Clock::update() {
    m_current = m_ticker->getTicks();
    m_delta = m_current - m_previous;
    m_previous = m_current;
}

float Clock::getDeltaTime() {
    return m_delta;
}
}
