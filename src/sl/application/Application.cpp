#include "Application.h"

#include "sl/core/perf/Profiler.h"
#include "sl/geometry/Geometry.hpp"
#include "sl/platform/shader/ShaderCompiler.hpp"
#include "sl/platform/gui/GUIImpl.h"


namespace sl::application {

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

    m_renderer = std::make_shared<rendering::Renderer>(m_window);
    m_renderer->init();

    m_guiImpl = platform::gui::GUIImpl::create(m_window->getHandle());
    m_guiProxy = std::make_shared<gui::GUIProxy>(m_guiImpl);

    platform::shader::ShaderCompiler::init();
    geometry::Geometry::init();

    m_sceneManager3D = std::make_shared<scene::SceneManager3D>(m_renderer);	
	m_sceneManager2D = std::make_shared<scene::SceneManager2D>(m_renderer);
}

void Application::update(float deltaTime, float time) {
    PRF_PROFILE_FUNCTION();

    m_window->update(deltaTime);
    m_context->update(deltaTime, time);
}

void Application::render() {
    PRF_PROFILE_FUNCTION();

    m_renderer->clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);
    m_context->render();
	renderGUI();
	m_renderer->swapBuffers();
}

void Application::renderGUI() {
    m_guiImpl->begin();
    m_context->renderGUI(m_guiProxy);
    m_guiImpl->end();
}

void Application::handleInput() {
    m_context->handleInput(m_input);
}

bool Application::isRunning() const {
    return not m_window->getShouldClose();
}

void Application::forceStop() {
    m_window->setShouldClose(true);
}
}
