#pragma once

#include <memory>
#include <optional>

#include "sl/app/Application.h"
#include "sl/app/ConfigLoader.h"
#include "sl/core/Logger.h"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/platform/Platform.hpp"
#include "sl/utils/Globals.h"

#include "sl/async/AsyncManager.hpp"
#include "sl/core/ClockManager.h"
#include "sl/core/Errors.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Input.h"
#include "sl/core/InputManager.h"
#include "sl/core/Logger.h"
#include "sl/core/Profiler.h"
#include "sl/core/sig/Signal.h"
#include "sl/geom/GeometryManager.h"
#include "sl/gfx/BufferManager.h"
#include "sl/gfx/ShaderManager.h"
#include "sl/gfx/TextureManager.h"
#include "sl/utils/Globals.h"

#include "sl/event/EventManager.h"

#include "sl/core/InputManager.h"
#include "sl/core/WindowManager.h"

#include "sl/platform/glfw/GlfwInput.h"

#include "sl/gui/GuiApi.h"

#include "sl/platform/gui/ImGuiApi.h"

namespace sl::app {

class Engine {
public:
    class Builder {
    public:
        Builder&& setConfigFile(const std::string& path) && {
            m_configPath = path;
            return std::move(*this);
        }

        Builder&& setPlatform(platform::Platform* platform) && {
            m_platform = platform;
            return std::move(*this);
        }

        std::unique_ptr<Engine> build() && {
            SL_ASSERT(m_platform != nullptr, "Platform is not set");
            SL_ASSERT(m_configPath.has_value(), "Config path is not set");

            SL_INFO("Creating Engine instance");
            return std::make_unique<Engine>(*m_configPath, m_platform);
        }

    private:
        platform::Platform* m_platform;
        std::optional<std::string> m_configPath;
    };

    void setApplication(std::unique_ptr<Application> application) {
        m_application = std::move(application);
        m_eventManager->registerListener(m_application.get());
    }

    explicit Engine(const std::string& configPath, platform::Platform* platform)
        : m_platform(platform)
        , m_application(nullptr) {

        m_window = m_platform->io.window.get(); //m_platform.windowFactory->create({ 1600, 900 }, "Starlight");
        m_window->init();
        m_window->makeContextCurrent();

        m_input = m_platform->io.input.get();
        m_input->init(m_window->getHandle());

        m_renderApi = m_platform->gpu.renderApi.get();
        m_renderApi->init();

        auto windowSize = m_window->getSize();
        auto viewport = gfx::ViewFrustum::Viewport { windowSize.width, windowSize.height };

        m_renderer = std::make_unique<gfx::Renderer>(m_renderApi, viewport);

        core::FileSystem fileSystem;
        SL_INFO("Loading config from file: {}.", configPath);
        GLOBALS().config = ConfigLoader {}.loadFromFile(configPath, fileSystem);

        SL_INFO("Creating managers");
        initManagers();

        GLOBALS().init();

        m_gui = std::make_unique<platform::gui::ImGuiApi>(m_window->getHandle());
        m_gui->addFont("/home/nek0/kapik/projects/starlight/res/fonts/fa-solid-900.ttf",
            ICON_MIN_FA, ICON_MAX_FA);
    }

    void run() {
        SL_ASSERT(m_application != nullptr, "Cannot run engine without set application");

        m_application->gui = m_gui.get(); // temporary
        m_application->onStart();

        while (m_application->isRunning()) {
            loopStep();
        }

        m_application->onStop();

        m_asyncManager->stop();
    }

private:
    void initManagers() {
        // clang-format off
        m_inputManager    = std::make_unique<core::InputManager>();
        m_windowManager   = std::make_unique<core::WindowManager>();
        m_asyncManager    = std::make_unique<async::AsyncManager>();
        m_clockManager    = std::make_unique<core::ClockManager>();
        m_shaderManager   = std::make_unique<gfx::ShaderManager>();
        m_bufferManager   = std::make_unique<gfx::BufferManager>();
        m_textureManager  = std::make_unique<gfx::TextureManager>();
        m_eventManager    = std::make_unique<event::EventManager>();
        m_geometryManager = std::make_unique<geom::GeometryManager>();
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
    }

    void loopStep() {
        SL_PROFILE_REGION("main-loop-step");

        float deltaTime = core::ClockManager::get()->getDeltaTime();

        m_window->update(deltaTime);
        m_application->update(deltaTime, core::ClockManager::get()->nowAsFloat());

        m_eventManager->update();
        m_inputManager->update();

        m_renderer->clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);
        m_application->render(*m_renderer);

        m_window->swapBuffers();

        m_asyncManager->update(deltaTime);
        core::ClockManager::get()->update();
    }

    platform::Platform* m_platform;

    core::Window* m_window;
    gfx::RenderApi* m_renderApi;
    core::Input* m_input;

    std::unique_ptr<gfx::Renderer> m_renderer;
    std::unique_ptr<Application> m_application;

    std::unique_ptr<core::InputManager> m_inputManager;

    std::unique_ptr<core::WindowManager> m_windowManager;
    std::unique_ptr<async::AsyncManager> m_asyncManager;
    std::unique_ptr<core::ClockManager> m_clockManager;
    std::unique_ptr<gfx::ShaderManager> m_shaderManager;
    std::unique_ptr<gfx::BufferManager> m_bufferManager;
    std::unique_ptr<gfx::TextureManager> m_textureManager;
    std::unique_ptr<event::EventManager> m_eventManager;
    std::unique_ptr<geom::GeometryManager> m_geometryManager;

    std::unique_ptr<gui::GuiApi> m_gui;
};

}