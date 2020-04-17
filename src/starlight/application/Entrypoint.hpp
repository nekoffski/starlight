#pragma once

#include <memory>
#include <type_traits>

#include <starlight/application/Application.h>
#include <starlight/core/BaseError.h>
#include <starlight/core/log/Logger.h>
#include <starlight/core/sig/Signal.h>
#include <starlight/platform/clock/Clock.h>

namespace starl::application {

// TODO: use concept to check if App is base of application::Application
template <typename App>
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
        core::log::initLogger(m_argc, m_argv);
        LOG(INFO) << "logger initialized";

        core::sig::setupSignalHandler(this);

        // TODO: load config
        try {
            LOG(INFO) << "creating and starting application instance";
            m_application = std::make_unique<App>(Application::Token{});
            m_application->onStart();

            if (not m_application->getContext())
                throw std::logic_error("Application context is null");

            while (m_application->isRunning()) {
                m_application->handleInput();
                m_application->update(m_clock.getDeltaTime());
                m_application->render();
                m_clock.update();
            }

            m_application->onStop();

        } catch (err::Exception& e) {
            LOG(ERROR) << "entrypoint catched unhandled error: " << e.toStr();
            return e.getGlobalCode();
        }
        LOG(INFO) << "shutdown gracefully";
        return 0;
    }

    void onSignal(int sig) override {
        LOG(WARNING) << "received signal, forcing application to stop";
        if (m_application)
            m_application->forceStop();
    }

private:
    std::unique_ptr<Application> m_application;
    platform::clock::Clock m_clock;
    int& m_argc;
    char**& m_argv;
};
}

#define STARLIGHT_ENTRYPOINT(App)                                                          \
    int main(int argc, char** argv) {                                                      \
        return std::make_unique<starl::application::Entrypoint<App>>(argc, argv)->start(); \
    }
