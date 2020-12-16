#include "Application.h"

#include "sl/core/PathManager.hpp"
#include "sl/core/Profiler.h"
#include "sl/event/EventBus.h"
#include "sl/geometry/Geometry.hpp"
#include "sl/platform/Platform.h"
#include "sl/platform/gpu/RenderAPI.h"
#include "sl/platform/shader/ShaderCompiler.hpp"

namespace sl::application {

static void initPaths() {
    sl::core::PathManager::registerResourcePath<sl::platform::shader::Shader>(SHADERS_DIR);
    sl::core::PathManager::registerResourcePath<sl::platform::texture::Texture>(TEXTURES_DIR);
    sl::core::PathManager::registerResourcePath<sl::platform::texture::Cubemap>(CUBEMAPS_DIR);
    sl::core::PathManager::registerResourcePath<sl::geometry::Model>(MODELS_DIR);
}

Application::Application(Application::Token&&) {
}

void Application::init() {
    initPaths();

    m_window = platform::window::Window::create(platform::window::WindowParams{
        platform::window::Viewport{ 1600, 900 }, "title" });
    m_window->init();
    m_window->setResizeCallback([](int width, int height) {
        SL_DEBUG("Window resized to: {}/{}", width, height);
        event::EventBus::emitEvent<event::WindowResizedEvent>(width, height);
    });

    auto windowHandle = m_window->getHandle();

    m_input = platform::input::Input::create(windowHandle);

    m_graphicsContext = platform::gpu::GraphicsContext::create(windowHandle);
    m_graphicsContext->init();
    m_graphicsContext->setViewport(1600, 900);

    m_renderer = std::make_shared<graphics::Renderer>(m_graphicsContext, platform::gpu::RenderAPI::create());
    m_renderer->setViewport(m_window->getParams().viewport);
    m_renderer->init();

    m_guiApi = platform::createGuiApi(windowHandle);

    platform::shader::ShaderCompiler::init();
    geometry::Geometry::init();

    m_sceneSystems = std::make_shared<scene::SceneSystems>(m_renderer);

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
    m_context->render(*m_sceneSystems);
    renderGui();
    m_renderer->swapBuffers();
}

void Application::renderGui() {
    m_guiApi->begin();
    m_context->renderGui(*m_guiApi);
    m_guiApi->end();
}

void Application::handleInput() {
    m_context->handleInput(m_input);
}

std::shared_ptr<ApplicationContext> Application::getActiveContext() const {
    return m_context;
}

void Application::handleEvents(event::EventPool& eventPool) {
    for (auto& event : eventPool.getEventsByCategory({ event::EventCategory::CORE })) {
        switch (event->getType()) {
        case event::EventType::WINDOW_RESIZED: {
            auto windowResizeEvent = event->as<event::WindowResizedEvent>();
            SL_INFO("{}/{}", windowResizeEvent->width, windowResizeEvent->height);
            m_renderer->setTemporaryViewport(windowResizeEvent->width, windowResizeEvent->height); // TODO: whhy temporary?
            break;
        }
        }
    }
}

void Application::switchContext(std::shared_ptr<ApplicationContext> context) {
    if (m_context)
        m_context->onDetach();
    m_context = context;
    m_context->onAttach();
}

bool Application::isRunning() const {
    return not m_window->getShouldClose();
}

void Application::forceStop() {
    m_window->setShouldClose(true);
}
}
