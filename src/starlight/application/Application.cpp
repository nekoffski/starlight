#include <starlight/application/Application.h>

namespace starl::application {

Application::Application(Application::Token&&) {
    m_window = platform::window::Window::create(platform::window::WindowParams{
        platform::window::Viewport{ 1600, 900 }, "title" });
    m_window->init();

    m_input = platform::input::Input::create(m_window->getHandle());

    m_lowLevelRenderer = std::make_unique<framework::graphics::LowLevelRenderer>(m_window);
    m_lowLevelRenderer->init();
}

void Application::update(float deltaTime) {
    m_window->update(deltaTime);
}

void Application::render() {
    m_lowLevelRenderer->begin();

    m_lowLevelRenderer->end();
}

void Application::handleInput() {
    if (m_input->isKeyPressed(STARL_KEY_ESCAPE))
        m_window->setShouldClose(true);
}

bool Application::isRunning() const {
    return not m_window->getShouldClose();
}

void Application::forceStop() {
    m_window->setShouldClose(true);
}
}
