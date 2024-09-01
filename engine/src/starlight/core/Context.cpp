#include "Context.hh"

#include "Log.hh"

namespace sl {

Context::LoggerInitializator::LoggerInitializator(const std::string& applicationName
) {
    sl::initLogging(applicationName);
}

Context::Context(const std::string& applicationName) :
    m_loggerInitializator(applicationName), m_window(m_windowImpl),
    m_input(m_windowImpl) {}

float Context::beginFrame() {
    const auto deltaTime = m_clock.getDeltaTime();

    m_windowImpl.update();
    m_eventBroker.dispatch();

    return deltaTime;
}

void Context::endFrame() {
    sl::disableVariableLogging();

    m_windowImpl.swapBuffers();

    m_clock.update();
    m_input.update();
}

Config& Context::getConfig() { return m_config; }
Window& Context::getWindow() { return m_window; }

}  // namespace sl
