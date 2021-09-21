#include "Engine.h"

#include <memory>
#include <optional>

#include <kc/core/Log.h>
#include <kc/core/Profiler.h>

#include "sl/app/Application.h"
#include "sl/app/ConfigLoader.h"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/platform/Platform.hpp"
#include "sl/utils/Globals.h"
#include <kc/core/Log.h>

#include "sl/async/AsyncManager.hpp"
#include "sl/core/ClockManager.h"
#include "sl/core/Errors.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Input.h"
#include "sl/core/InputManager.h"

#include "sl/core/sig/Signal.h"
#include "sl/geom/GeometryManager.h"
#include "sl/gfx/BufferManager.h"
#include "sl/gfx/ShaderManager.h"
#include "sl/gfx/TextureManager.h"
#include "sl/utils/Globals.h"

#include "sl/core/InputManager.h"
#include "sl/core/WindowManager.h"
#include "sl/event/EventManager.h"

namespace sl::app {

Engine::Builder&& Engine::Builder::setConfig(utils::Config* config) && {
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

Engine::Engine(utils::Config* config, platform::Platform* platform)
    : EventListener("Engine")
    , m_config(config)
    , m_platform(platform)
    , m_application(nullptr) {

    initLowLevelComponents();
    initManagers();
}

Engine::~Engine() {
    if (m_asyncManager != nullptr)
        m_asyncManager->stop();
}

void Engine::initLowLevelComponents() {
    m_window = m_platform->io.window.get();

    m_window->init();
    m_window->makeContextCurrent();
    m_window->setResizeCallback([](void*, int width, int height) {
        event::EventManager::get()->emit<event::WindowResizedEvent>(width, height).toAll();
    });

    m_input = m_platform->io.input.get();
    m_input->init(m_window->getHandle());

    m_renderApi = m_platform->gpu.renderApi.get();
    m_renderApi->init();

    auto windowSize = m_window->getSize();
    auto viewport = gfx::ViewFrustum::Viewport { windowSize.width, windowSize.height };

    m_renderer = std::make_unique<gfx::Renderer>(m_renderApi, viewport);
}

void Engine::initManagers() {
    LOG_INFO("Initializing managers");

    // clang-format off
    m_inputManager    = std::make_unique<core:: InputManager>();
    m_windowManager   = std::make_unique<core:: WindowManager>();
    m_asyncManager    = std::make_unique<async::AsyncManager>();
    m_clockManager    = std::make_unique<core:: ClockManager>();
    m_shaderManager   = std::make_unique<gfx::  ShaderManager>();
    m_bufferManager   = std::make_unique<gfx::  BufferManager>();
    m_textureManager  = std::make_unique<gfx::  TextureManager>();
    m_eventManager    = std::make_unique<event::EventManager>();
    m_geometryManager = std::make_unique<geom:: GeometryManager>();
    // clang-format on

    m_inputManager->setKeyboard(m_input);
    m_inputManager->setMouse(m_input);

    m_shaderManager->setShaderCompiler(m_platform->gpu.shaderCompiler.get());
    m_shaderManager->setShaderFactory(m_platform->gpu.shaderFactory.get());

    m_bufferManager->setElementBufferFactory(m_platform->gpu.elementBufferFactory.get());
    m_bufferManager->setVertexBufferFactory(m_platform->gpu.vertexBufferFactory.get());
    m_bufferManager->setRenderBufferFactory(m_platform->gpu.renderBufferFactory.get());
    m_bufferManager->setFrameBufferFactory(m_platform->gpu.frameBufferFactory.get());
    m_bufferManager->setVertexArrayFactory(m_platform->gpu.vertexArrayFactory.get());

    m_textureManager->setCubemapFactory(m_platform->gpu.cubemapFactory.get());
    m_textureManager->setTextureFactory(m_platform->gpu.textureFactory.get());
    m_textureManager->setImageFactory(m_platform->imageFactory.get());

    m_geometryManager->setModelLoader(m_platform->modelLoader.get());

    m_windowManager->setActiveWindow(m_window);
    m_asyncManager->start();

    m_eventManager->registerListener(this);
}

void Engine::initGlobalState() {
    GLOBALS().config = *m_config;
    GLOBALS().init();
}

void Engine::setApplication(std::unique_ptr<Application> application) {
    m_application = std::move(application);
    m_eventManager->registerListener(m_application.get());
}

void Engine::handleEvents(const kc::event::EventProvider& eventProvider) {
    auto events = eventProvider.getByCategories<event::CoreCategory, event::EditorCategory>();

    for (auto& event : events) {
        if (event->is<event::ChangeViewportEvent>())
            m_renderer->setViewport(
                event->as<event::ChangeViewportEvent>()->viewport);
    }
}

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
    float deltaTime = core::ClockManager::get()->getDeltaTime();

    m_window->update(deltaTime);
    m_application->update(deltaTime, core::ClockManager::get()->getClock().nowAsFloat());

    m_eventManager->update();
    m_inputManager->update();

    m_asyncManager->update(deltaTime);
    core::ClockManager::get()->update();
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

}