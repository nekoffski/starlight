#include "Context.h"

#include "utils/Log.h"

namespace sl {

Context::LoggerInitializator::LoggerInitializator(const std::string& applicationName) {
    sl::initLogging(applicationName);
}

Context::Context(const std::string& applicationName) :
    m_loggerInitializator(applicationName), m_windowManager(&m_window) {}

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
