#pragma once

#include <concepts>
#include <memory>

#include "Application.h"
#include "sl/core/sig/Signal.h"

namespace sl::application {

class Entrypoint : public core::sig::SignalHandler {
public:
    template <typename App>
    requires std::derived_from<App, Application> static std::shared_ptr<Entrypoint> create(int& argc, char**& argv) {
        return std::make_unique<Entrypoint>(
            argc, argv, std::make_unique<App>(Application::Token{}));
    }

    explicit Entrypoint(int& argc, char**& argv, std::unique_ptr<Application> application);

    int start();
    void onSignal(int sig);

private:
    std::shared_ptr<async::timer::Timer> m_profilerTimer;
    std::unique_ptr<Application> m_application;
    int& m_argc;
    char**& m_argv;
};
}

#define STARLIGHT_ENTRYPOINT(App)                                             \
    int main(int argc, char** argv) {                                         \
        return sl::application::Entrypoint::create<App>(argc, argv)->start(); \
    }
