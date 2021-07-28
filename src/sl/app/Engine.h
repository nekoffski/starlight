#pragma once

#include <memory>
#include <optional>

#include "sl/app/Application.h"
#include "sl/app/ConfigLoader.h"
#include "sl/core/Logger.h"
#include "sl/platform/Platform.h"
#include "sl/utils/Globals.h"

#include "sl/async/AsyncManager.hpp"
#include "sl/core/ClockManager.h"
#include "sl/core/Errors.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Logger.h"
#include "sl/core/Profiler.h"
#include "sl/core/sig/Signal.h"
#include "sl/gfx/BufferManager.h"
#include "sl/gfx/GraphicsContext.h"
#include "sl/gfx/ShaderManager.h"
#include "sl/utils/Globals.h"

#include "sl/core/InputManager.h"
#include "sl/core/WindowManager.h"

namespace sl::app {

class Engine {
public:
    class Builder {
    public:
        Builder&& setConfigFile(const std::string& path) && {
            m_configPath = path;
            return std::move(*this);
        }

        Builder&& setPlatform(platform::Platform platform) && {
            m_platform = std::move(platform);
            return std::move(*this);
        }

        std::unique_ptr<Engine> build() && {
            SL_ASSERT(m_platform.has_value(), "Platform is not set");
            SL_ASSERT(m_configPath.has_value(), "Config path is not set");

            SL_INFO("Creating Engine instance");
            return std::make_unique<Engine>(*m_configPath, std::move(*m_platform));
        }

    private:
        std::optional<platform::Platform> m_platform;
        std::optional<std::string> m_configPath;
    };

    void setApplication(std::unique_ptr<Application> application) {
        m_application = std::move(application);
    }

    explicit Engine(const std::string& configPath, platform::Platform platform)
        : m_platform(std::move(platform))
        , m_application(nullptr) {

        core::initLogging();

        m_window = m_platform.windowFactory->create({ 1600, 900 }, "Starlight");
        m_window->init();

        m_gfxContext = m_platform.graphicsContextFactory->create(m_window->getHandle());

        SL_INFO("Creating managers");
        initManagers();

        core::FileSystem fileSystem;
        SL_INFO("Loading config from file: {}.", configPath);
        GLOBALS().config = ConfigLoader {}.loadFromFile(configPath, fileSystem);
    }

    void run() {
        SL_ASSERT(m_application != nullptr, "Cannot run engine without set application");

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
        m_inputManager  = std::make_unique<core::InputManager>();
        m_windowManager = std::make_unique<core::WindowManager>();
        m_asyncManager  = std::make_unique<async::AsyncManager>();
        m_clockManager  = std::make_unique<core::ClockManager>();
        m_shaderManager = std::make_unique<gfx::ShaderManager>();
        m_bufferManager = std::make_unique<gfx::BufferManager>();
        // clang-format on

        m_shaderManager->setShaderCompiler(m_platform.shaderCompilerFactory->create());
        m_shaderManager->setShaderFactory(m_platform.shaderFactory.get());

        m_bufferManager->setElementBufferFactory(m_platform.elementBufferFactory.get());
        m_bufferManager->setVertexBufferFactory(m_platform.vertexBufferFactory.get());
        m_bufferManager->setRenderBufferFactory(m_platform.renderBufferFactory.get());
        m_bufferManager->setFrameBufferFactory(m_platform.frameBufferFactory.get());
        m_bufferManager->setVertexArrayFactory(m_platform.vertexArrayFactory.get());

        m_windowManager->setActiveWindow(m_window.get());
        m_asyncManager->start();
    }

    void loopStep() {
        SL_PROFILE_REGION("main-loop-step");

        float deltaTime = core::ClockManager::get()->getDeltaTime();

        m_application->update(deltaTime, core::ClockManager::get()->nowAsFloat());
        m_application->render(*m_renderer);

        m_asyncManager->update(deltaTime);
        core::ClockManager::get()->update();
    }

    platform::Platform m_platform;

    std::unique_ptr<core::Window> m_window;
    std::unique_ptr<gfx::LowLevelRenderer> m_renderer;
    std::shared_ptr<gfx::GraphicsContext> m_gfxContext;

    std::unique_ptr<Application> m_application;

    std::unique_ptr<core::InputManager> m_inputManager;

    std::unique_ptr<core::WindowManager> m_windowManager;
    std::unique_ptr<async::AsyncManager> m_asyncManager;
    std::unique_ptr<core::ClockManager> m_clockManager;
    std::unique_ptr<gfx::ShaderManager> m_shaderManager;
    std::unique_ptr<gfx::BufferManager> m_bufferManager;
};

}