#include <starlight/platform/clock/Clock.h>

#include <GLFW/glfw3.h>

static constexpr float DIVIDER = 1000.0f;

namespace starl::platform::clock {

Clock::Clock()
    : m_current(0.0f)
    , m_previous(0.0f)
    , m_delta(0.0f) {
}

void Clock::update() {
    m_current = getTicks();
    m_delta = m_current - m_previous;
    m_previous = m_current;
}

float Clock::getDeltaTime() {
    return m_delta;
}

float Clock::getTicks() {
    return glfwGetTime();
}
}
