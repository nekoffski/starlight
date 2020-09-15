#include "Application.h"

#include "starlight/core/perf/Profiler.hpp"
#include "starlight/geometry/Geometry.hpp"
#include "starlight/platform/shader/ShaderCompiler.hpp"

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

    m_lowLevelRenderer = std::make_unique<rendering::renderer::lowlevel::LowLevelRenderer>(m_window);
    m_lowLevelRenderer->init();

    m_guiAdapter = std::make_shared<platform::gui::GUIAdapter>(m_window->getHandle());
    m_guiProxy = std::make_shared<gui::GUIProxy>(m_guiAdapter);

    platform::shader::ShaderCompiler::init();
    geometry::Geometry::init();

    m_renderer = rendering::RendererProxy::create(*m_lowLevelRenderer);
    m_sceneManager = std::make_shared<scene::SceneManager>(m_renderer);
}

void Application::update(float deltaTime, float time) {
    m_window->update(deltaTime);
    m_context->update(deltaTime, time);
}

void Application::render() {
    static auto t1 = PROFILER_CREATE_TIMER("Context render");
    static auto t2 = PROFILER_CREATE_TIMER("GUI render");
    static auto t3 = PROFILER_CREATE_TIMER("LLRenderer begin");
    static auto t4 = PROFILER_CREATE_TIMER("LLRenderer end");

    PROFILER_TIMER_BEGIN(t3);
    m_lowLevelRenderer->begin();
    PROFILER_TIMER_END(t3);

    PROFILER_TIMER_BEGIN(t1);
    m_context->render();
    PROFILER_TIMER_END(t1);

    PROFILER_TIMER_BEGIN(t2);
    renderGUI();
    PROFILER_TIMER_END(t2);

    PROFILER_TIMER_BEGIN(t4);
    m_lowLevelRenderer->end();
    PROFILER_TIMER_END(t4);
}

void Application::renderGUI() {
    static auto t1 = PROFILER_CREATE_TIMER("GUI Begin");
    static auto t2 = PROFILER_CREATE_TIMER("GUI Context render");
    static auto t3 = PROFILER_CREATE_TIMER("GUI End");

    PROFILER_TIMER_BEGIN(t1);
    m_guiAdapter->begin();
    PROFILER_TIMER_END(t1);

    PROFILER_TIMER_BEGIN(t2);
    m_context->renderGUI(m_guiProxy);
    PROFILER_TIMER_END(t2);

    PROFILER_TIMER_BEGIN(t3);
    m_guiAdapter->end();
    PROFILER_TIMER_END(t3);
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
