#include "Entrypoint.h"

#include <concepts>
#include <memory>
#include <thread>
#include <type_traits>

#include "sl/async/AsyncEngine.hpp"
#include "sl/core/Clock.h"
#include "sl/core/FileSystem.h"
#include "sl/core/Logger.h"
#include "sl/core/Profiler.h"
#include "sl/core/error/Errors.hpp"
#include "sl/core/sig/Signal.h"
#include "sl/event/EventBus.h"
#include "sl/platform/clock/StdClockImpl.h"

namespace sl::application {

Entrypoint::Entrypoint(int& argc, char**& argv, std::unique_ptr<Application> application)
    : m_argc(argc)
    , m_argv(argv)
    , m_application(std::move(application)) {
}

int Entrypoint::start() {
    core::sig::setupSignalHandler(this);
    core::initLogging();
    SL_INFO("Initialized logging");

    core::Clock::setClockImpl<platform::clock::StdClockImpl>();
    m_profilerTimer = async::AsyncEngine::createTimer(PROFILER_PRINT_INTERVAL);

    // TODO: load config

    try {
        SL_INFO("creating and starting application instance");
        m_application->init();
        m_application->onStart();

        SL_ASSERT(m_application->getActiveContext(), "Application context is null");

        while (m_application->isRunning()) {
            {
                PRF_PROFILE_REGION("main-loop");

                float deltaTime = core::Clock::getDeltaTime();

                m_application->handleInput();
                m_application->update(deltaTime, core::Clock::now()->value());
                m_application->render();

                event::EventBus::spreadEvents();
                async::AsyncEngine::update(deltaTime);
                core::Clock::update();
            }

            if (not m_profilerTimer->asyncSleep())
                core::Profiler::printResults();
        }

        m_application->onStop();

    } catch (core::error::Error& e) {
        SL_ERROR("entrypoint catched unhandled error: {}", e.toString());
        return e.getErrorCode<int>();
    }

    core::Profiler::saveResults("./logs/");
    SL_INFO("shutdown gracefully");
    return 0;
}

void Entrypoint::onSignal(int sig) {
    SL_WARN("received signal, forcing application to stop");
    if (m_application)
        m_application->forceStop();
}
}
