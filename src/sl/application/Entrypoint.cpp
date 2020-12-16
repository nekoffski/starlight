#include "Entrypoint.h"

#include <concepts>
#include <memory>
#include <thread>
#include <type_traits>

#include "sl/async/AsyncEngine.hpp"
#include "sl/core/BaseError.h"
#include "sl/core/FileSystem.h"
#include "sl/core/Logger.h"
#include "sl/core/Profiler.h"
#include "sl/core/sig/Signal.h"
#include "sl/event/EventBus.h"
#include "sl/platform/time/Clock.h"
#include "sl/platform/time/impl/StdClockImpl.h"

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

    platform::time::Clock::setClockImpl<platform::time::impl::StdClockImpl>();
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

                float deltaTime = platform::time::Clock::getDeltaTime();

                m_application->handleInput();
                m_application->update(deltaTime, platform::time::Clock::now()->value());
                m_application->render();

                event::EventBus::spreadEvents();
                async::AsyncEngine::update(deltaTime);
                platform::time::Clock::update();
            }

            if (not m_profilerTimer->asyncSleep())
                core::Profiler::printResults();
        }

        m_application->onStop();

    } catch (err::Exception& e) {
        SL_ERROR("entrypoint catched unhandled error: {}", e.toStr());
        return e.getGlobalCode();
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
