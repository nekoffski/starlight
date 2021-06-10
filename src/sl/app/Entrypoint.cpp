#include "Entrypoint.h"

#include <concepts>
#include <memory>
#include <thread>
#include <type_traits>

#include "ConfigLoader.h"
#include "sl/async/AsyncEngine.hpp"
#include "sl/core/Clock.h"
#include "sl/core/Errors.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Logger.h"
#include "sl/core/Profiler.h"
#include "sl/core/sig/Signal.h"
#include "sl/utils/Globals.h"

namespace sl::app {

Entrypoint::Entrypoint(int argc, char** argv, std::unique_ptr<Application> application)
    : m_argc(argc)
    , m_argv(argv)
    , m_application(std::move(application)) {
}

int Entrypoint::start() {
    try {
        core::initLogging();
        SL_INFO("Initialized logging.");

        SL_INFO("Setting up signal handler.");
        core::sig::setupSignalHandler(this);

        loadConfig();

        SL_INFO("Setting up async engine.");
        ASYNC_ENGINE().init();

        Application::initDefaultFactories();

        SL_INFO("Initializing and starting application.");
        m_application->init();
        m_application->onStart();

        SL_ASSERT(m_application->getActiveContext(), "Application context is null.");

        static constexpr float loggerSavingInterval = 1.0f;
        auto saveProfilerResultsTimer = ASYNC_ENGINE().createTimer(loggerSavingInterval);

        SL_INFO("Starting starlight main loop.");
        while (m_application->isRunning()) {
            loopStep();

            if (not saveProfilerResultsTimer->asyncSleep())
                PROFILER().saveResults();
        }

        m_application->onStop();
        ASYNC_ENGINE().deinit();

    } catch (core::Error& e) {
        SL_ERROR("Entrypoint catched unhandled error: {}.", e.as<std::string>());
        return e.getErrorCode<int>();
    }

    PROFILER().saveResults();
    SL_INFO("Shutdown gracefully.");
    return 0;
}

void Entrypoint::loopStep() {
    SL_PROFILE_REGION("main-loop-step");

    float deltaTime = CLOCK().getDeltaTime();

    m_application->update(deltaTime, CLOCK().nowAsFloat());
    m_application->render();

    ASYNC_ENGINE().update(deltaTime);
    CLOCK().update();
}

void Entrypoint::loadConfig() {
    const std::string defaultConfigPath = "./starlight.json";
    std::string configFilePath = m_argc > 1 ? m_argv[1] : defaultConfigPath;

    core::FileSystem fileSystem;
    SL_INFO("Loading config from file: {}.", configFilePath);
    GLOBALS().config = ConfigLoader {}.loadFromFile(configFilePath, fileSystem);
}

void Entrypoint::onSignal(int sig) {
    SL_WARN("Received signal, forcing starlight application to stop.");
    if (m_application)
        m_application->forceStop();
}
}
