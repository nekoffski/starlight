#include "Context.h"

namespace sl {

Context::Context() : m_windowManager(&m_window) {}

Window* Context::getWindow() { return &m_window; }

float Context::beginFrame() {
    const auto deltaTime = m_timeManager.getDeltaTime();

    m_window.update();

    return deltaTime;
}

void Context::endFrame() {
    m_window.swapBuffers();
    m_eventManager.dispatch();
    m_timeManager.update();
}

Config* Context::getConfig() { return &m_config; }

}  // namespace sl
