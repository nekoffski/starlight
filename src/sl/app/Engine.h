#pragma once

#include <memory>
#include <optional>

#include "sl/app/Application.h"
#include "sl/app/ConfigLoader.h"
#include "sl/core/Logger.h"
#include "sl/platform/Platform.h"
#include "sl/utils/Globals.h"

#include "sl/async/AsyncEngine.hpp"
#include "sl/core/Clock.h"
#include "sl/core/Errors.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Logger.h"
#include "sl/core/Profiler.h"
#include "sl/core/sig/Signal.h"
#include "sl/utils/Globals.h"

#include "sl/core/InputManager.h"

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

        SL_INFO("Creating managers");
        initManagers();

        SL_INFO("Initializing factories");
        Application::initDefaultFactories();

        core::FileSystem fileSystem;
        SL_INFO("Loading config from file: {}.", configPath);
        GLOBALS().config = ConfigLoader {}.loadFromFile(configPath, fileSystem);
    }

    void run() {
        SL_ASSERT(m_application != nullptr, "Cannot run engine without set application");

        m_application->init();
        m_application->onStart();

        while (m_application->isRunning()) {
            loopStep();
        }

        m_application->onStop();
        ASYNC_ENGINE().deinit();
    }

    void initManagers() {
        m_inputManager = std::make_unique<core::InputManager>();

        SL_INFO("Creating async engine");
        ASYNC_ENGINE().init();
    }

    void loopStep() {
        SL_PROFILE_REGION("main-loop-step");

        float deltaTime = CLOCK().getDeltaTime();

        m_application->update(deltaTime, CLOCK().nowAsFloat());
        m_application->render();

        ASYNC_ENGINE().update(deltaTime);
        CLOCK().update();
    }

private:
    platform::Platform m_platform;

    std::unique_ptr<Application> m_application;

    std::unique_ptr<core::InputManager> m_inputManager;
};

}