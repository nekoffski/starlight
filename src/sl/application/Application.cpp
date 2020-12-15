#include "Application.h"

#include "sl/core/perf/Profiler.h"
#include "sl/event/EventBus.h"
#include "sl/geometry/Geometry.hpp"
#include "sl/platform/gpu/RenderAPI.h"
#include "sl/platform/gui/GuiImpl.h"
#include "sl/platform/shader/ShaderCompiler.hpp"

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
    m_window->setResizeCallback([](int width, int height) {
        SL_DEBUG("Window resized to: {}/{}", width, height);
        event::EventBus::emitEvent<event::WindowResizedEvent>(width, height);
    });

    auto windowHandle = m_window->getHandle();

    m_graphicsContext = platform::gpu::GraphicsContext::create(windowHandle);
    m_input = platform::input::Input::create(windowHandle);

    m_renderer = std::make_shared<rendering::Renderer>(m_graphicsContext, platform::gpu::RenderAPI::create());
    m_renderer->init();

    m_guiImpl = platform::gui::GuiImpl::create(m_window->getHandle());
    m_guiProxy = std::make_shared<gui::GuiProxy>(m_guiImpl);

    platform::shader::ShaderCompiler::init();
    geometry::Geometry::init();

    m_sceneManager3D = std::make_shared<scene::SceneManager3D>(m_renderer);
    m_sceneManager2D = std::make_shared<scene::SceneManager2D>(m_renderer);

    event::EventBus::registerEventObserver(this);
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
    renderGui();
    m_renderer->swapBuffers();
}

void Application::renderGui() {
    m_guiImpl->begin();
    m_context->renderGui(*m_guiProxy);
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
