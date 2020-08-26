#pragma once

#include <concepts>
#include <memory>
#include <thread>
#include <type_traits>

#include "Application.h"
#include "starlight/core/BaseError.h"
#include "starlight/core/log/Logger.h"
#include "starlight/core/perf/Profiler.hpp"
#include "starlight/core/sig/Signal.h"
#include "starlight/platform/time/Clock.h"
#include "starlight/platform/time/impl/StdClockImpl.h"

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
        core::perf::Profiler::init("profiler.txt");

        // TODO: load config

        auto t1 = PROFILER_CREATE_TIMER("Main loop");
        auto t2 = PROFILER_CREATE_TIMER("Render");
        auto t3 = PROFILER_CREATE_TIMER("Update");

        try {
            SL_INFO("creating and starting application instance");
            m_application = std::make_unique<App>(Application::Token{});
            m_application->init();
            m_application->onStart();

            if (not m_application->getActiveContext())
                throw std::logic_error("Application context is null");

            while (m_application->isRunning()) {
                PROFILER_TIMER_BEGIN(t1);
                m_application->handleInput();
                PROFILER_TIMER_BEGIN(t3);
                m_application->update(platform::time::Clock::getDeltaTime());
                PROFILER_TIMER_END(t3);

                PROFILER_TIMER_BEGIN(t2);
                m_application->render();
                PROFILER_TIMER_END(t2);
                platform::time::Clock::update();

                PROFILER_TIMER_END(t1);
                PROFILER_PRINT(1);
            }

            m_application->onStop();

        } catch (err::Exception& e) {
            SL_ERROR("entrypoint catched unhandled error: {}", e.toStr());
            return e.getGlobalCode();
        }

        core::perf::Profiler::shutdown();
        SL_INFO("shutdown gracefully");
        return 0;
    }

    void onSignal(int sig) override {
        SL_WARN("received signal, forcing application to stop");
        if (m_application)
            m_application->forceStop();
    }

private:
    std::unique_ptr<Application> m_application;
    int& m_argc;
    char**& m_argv;
};
}

#define STARLIGHT_ENTRYPOINT(App)                                                       \
    int main(int argc, char** argv) {                                                   \
        return std::make_unique<sl::application::Entrypoint<App>>(argc, argv)->start(); \
    }
