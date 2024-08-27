#include "Context.hh"

#include "Log.hh"

namespace sl {

Context::LoggerInitializator::LoggerInitializator(const std::string& applicationName
) {
    sl::initLogging(applicationName);
}

Context::Context(const std::string& applicationName) :
    m_loggerInitializator(applicationName), m_windowManager(&m_window) {}

float Context::beginFrame() {
    const auto deltaTime = m_timeManager.getDeltaTime();

    m_window.update();
    m_eventBroker.dispatch();

    return deltaTime;
}

void Context::endFrame() {
    sl::disableVariableLogging();

    m_window.swapBuffers();

    // TODO: no managers!!
    m_timeManager.update();
    m_windowManager.update();
}

Config& Context::getConfig() { return m_config; }
Window& Context::getWindow() { return m_window; }

}  // namespace sl
