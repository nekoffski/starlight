#pragma once

#include <concepts>
#include <memory>
#include <thread>
#include <type_traits>

#include "Application.h"
#include "sl/core/BaseError.h"
#include "sl/core/async/AsyncEngine.hpp"
#include "sl/core/log/Logger.h"
#include "sl/core/perf/Profiler.h"
#include "sl/core/sig/Signal.h"
#include "sl/platform/time/Clock.h"
#include "sl/platform/time/impl/StdClockImpl.h"

namespace sl::application {

template <Applicable App>
class Entrypoint : public core::sig::SignalHandler {
public:
    explicit Entrypoint(int& argc, char**& argv)
        : m_argc(argc)
        , m_argv(argv)
        , m_application(nullptr) {
        // TODO: replace with concept as soon as c++20 arrives
        if (!std::is_base_of<Application, App>())
            throw std::logic_error("App has to be derived from application::Application!");
    }

    int start() {
        core::sig::setupSignalHandler(this);
        core::log::initLogging();
        SL_INFO("Initialized logging");

        platform::time::Clock::setClockImpl<platform::time::impl::StdClockImpl>();

        m_profilerTimer = core::async::AsyncEngine::createTimer(PROFILER_PRINT_INTERVAL);

        // TODO: load config

        try {
            SL_INFO("creating and starting application instance");
            m_application = std::make_unique<App>(Application::Token{});
            m_application->init();
            m_application->onStart();

            if (not m_application->getActiveContext())
                throw std::logic_error("Application context is null");

            while (m_application->isRunning()) {
                {
                    PRF_PROFILE_REGION("main-loop");

                    float deltaTime = platform::time::Clock::getDeltaTime();

                    m_application->handleInput();
                    m_application->update(deltaTime, platform::time::Clock::now()->value());
                    m_application->render();

                    core::async::AsyncEngine::update(deltaTime);
                    platform::time::Clock::update();
                }

				if (not m_profilerTimer->asyncSleep())
					core::perf::Profiler::printResults();
            }

            m_application->onStop();

        } catch (err::Exception& e) {
            SL_ERROR("entrypoint catched unhandled error: {}", e.toStr());
            return e.getGlobalCode();
        }

        core::perf::Profiler::saveResults("./logs/");
        SL_INFO("shutdown gracefully");
        return 0;
    }

    void onSignal(int sig) override {
        SL_WARN("received signal, forcing application to stop");
        if (m_application)
            m_application->forceStop();
    }

private:
	std::shared_ptr<core::async::Timer> m_profilerTimer;
    std::unique_ptr<Application> m_application;
    int& m_argc;
    char**& m_argv;
};
}

#define STARLIGHT_ENTRYPOINT(App)                                                       \
    int main(int argc, char** argv) {                                                   \
        return std::make_unique<sl::application::Entrypoint<App>>(argc, argv)->start(); \
    }
