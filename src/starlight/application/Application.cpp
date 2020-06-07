#include <starlight/application/Application.h>
#include <starlight/platform/shader/ShaderCompiler.hpp>

namespace starl::application {

Application::Application(Application::Token&&) {
}

// TODO: refactor this, it left object in invalid state but as we need to define some actions by end user
// we cannot do it ctor as we can't call overriden methods there :<
void Application::init() {
    preInit();

    m_window = platform::window::Window::create(platform::window::WindowParams{
        platform::window::Viewport{ 1600, 900 }, "title" });
    m_window->init();

    m_input = platform::input::Input::create(m_window->getHandle());

    m_lowLevelRenderer = std::make_unique<framework::graphics::LowLevelRenderer>(m_window);
    m_lowLevelRenderer->init();

    m_guiAdapter = std::make_shared<platform::gui::GUIAdapter>(m_window->getHandle());
    m_guiProxy = std::make_shared<gui::GUIProxy>(m_guiAdapter);

    platform::shader::ShaderCompiler::init();

    m_renderer = rendering::RendererProxy::create(*m_lowLevelRenderer, m_assetManager);
    m_sceneManager = std::make_shared<scene::SceneManager>(m_renderer);
}

void Application::update(float deltaTime) {
    m_window->update(deltaTime);
    m_context->update(deltaTime);
}

void Application::render() {
    m_lowLevelRenderer->begin();
    m_context->render();
    renderGUI();
    m_lowLevelRenderer->end();
}

void Application::renderGUI() {
    m_guiAdapter->begin();
    m_context->renderGUI(m_guiProxy);
    m_guiAdapter->end();
}

void Application::handleInput() {
    if (m_input->isKeyPressed(STARL_KEY_ESCAPE))
        m_window->setShouldClose(true);
    m_context->handleInput(m_input);
}

bool Application::isRunning() const {
    return not m_window->getShouldClose();
}

void Application::forceStop() {
    m_window->setShouldClose(true);
}
}
