#include "Engine.h"

#include <kc/core/Log.h>
#include <kc/core/Profiler.h>
#include <kc/sig/Signal.h>

#include <memory>
#include <optional>

#include "sl/app/Application.h"
#include "sl/async/AsyncManager.hpp"
#include "sl/cfg/Config.h"
#include "sl/core/ClockManager.h"
#include "sl/core/Errors.hpp"
#include "sl/core/Input.h"
#include "sl/core/InputManager.h"
#include "sl/core/WindowManager.h"
#include "sl/event/EventManager.h"
#include "sl/geom/GeometryManager.h"
#include "sl/gfx/BufferManager.h"
#include "sl/gfx/ShaderManager.h"
#include "sl/gfx/TextureManager.h"
#include "sl/glob/Globals.h"
#include "sl/gui/Utils.h"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/platform/Platform.hpp"

namespace sl::app {

Engine::Builder&& Engine::Builder::setConfig(cfg::Config* config) && {
    m_config = config;
    return std::move(*this);
}

Engine::Builder&& Engine::Builder::setPlatform(platform::Platform* platform) && {
    m_platform = platform;
    return std::move(*this);
}

std::unique_ptr<Engine> Engine::Builder::build() && {
    ASSERT(m_platform != nullptr, "Platform is not set");
    ASSERT(m_config != nullptr, "Config is not set");

    LOG_TRACE("Creating Engine instance");
    return std::make_unique<Engine>(m_config, m_platform);
}

Engine::Engine(cfg::Config* config, platform::Platform* platform)
    : EventListener("Engine"),
      m_config(config),
      m_platform(platform),
      m_application(nullptr)
      // managers
      ,
      m_textureManager(m_platform->gpu.textureFactory.get(), m_platform->gpu.cubemapFactory.get(),
                       m_platform->imageFactory.get()),
      m_shaderManager(m_platform->gpu.shaderCompiler.get(), m_platform->gpu.shaderFactory.get()) {
    initLowLevelComponents();
    initManagers();

    auto& eventManager = sl::event::EventManager::get();
    eventManager.registerListener(this);
    eventManager.registerListener(&m_inputManager);
    eventManager.registerListener(&m_windowManager);
}

Engine::~Engine() = default;

void Engine::initLowLevelComponents() {
    m_window = m_platform->io.window.get();

    m_window->init();
    m_window->makeContextCurrent();
    m_window->setResizeCallback([](void*, int width, int height) {
        event::EventManager::get().emit<event::WindowResizedEvent>(width, height).toAll();
    });

    m_input = m_platform->io.input.get();
    m_input->init(m_window->getHandle());

    m_renderApi = m_platform->gpu.renderApi.get();
    m_renderApi->init();

    auto windowSize = m_window->getSize();
    auto viewport = gfx::Viewport{windowSize.width, windowSize.height};

    m_renderer = std::make_unique<gfx::Renderer>(m_renderApi, viewport);
}

void Engine::initManagers() {
    LOG_INFO("Initializing managers");

    m_inputManager.setKeyboard(m_input);
    m_inputManager.setMouse(m_input);

    m_bufferManager.setElementBufferFactory(m_platform->gpu.elementBufferFactory.get());
    m_bufferManager.setVertexBufferFactory(m_platform->gpu.vertexBufferFactory.get());
    m_bufferManager.setRenderBufferFactory(m_platform->gpu.renderBufferFactory.get());
    m_bufferManager.setFrameBufferFactory(m_platform->gpu.frameBufferFactory.get());
    m_bufferManager.setVertexArrayFactory(m_platform->gpu.vertexArrayFactory.get());

    m_geometryManager.setModelLoader(m_platform->modelLoader.get());

    m_windowManager.setActiveWindow(m_window);
}

void Engine::initGlobalState() {
    m_globals.config = *m_config;
    m_globals.rendererInfo = m_renderApi->getRendererInfo();
    m_globals.init();
}

void Engine::setApplication(Application* application) {
    m_application = application;
    m_eventManager.registerListener(m_application);
}

void Engine::handleEvents(const kc::event::EventProvider& eventProvider) {}

void Engine::run() {
    ASSERT(m_application != nullptr, "Cannot run engine without set application");

    kc::core::Profiler profiler;

    while (m_application->isRunning()) {
        loopStep();

        // clang-format off
        #if 0
        LOG_TRACE("\n\n{}\n\n", profiler.formatTimers());
        #endif
        // clang-format on
    }

    LOG_INFO("Main loop ended");
}

void Engine::update() {
    float deltaTime = core::ClockManager::get().getDeltaTime();

    m_window->update(deltaTime);
    m_application->update(deltaTime, core::ClockManager::get().getClock().nowAsFloat());

    m_eventManager.update();
    m_inputManager.update();

    m_asyncManager.update(deltaTime);
    core::ClockManager::get().update();
}

void Engine::render() {
    m_renderer->clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);

    m_application->render(*m_renderer);
    m_window->swapBuffers();
}

void Engine::loopStep() {
    PROFILE_REGION(main_loop_step);

    update();
    render();
}

}  // namespace sl::app