#pragma once

#include <concepts>
#include <memory>

#include "Application.h"
#include "sl/core/sig/Signal.h"

namespace sl::application {

class Entrypoint : public core::sig::SignalHandler {
public:
    // clang-format off
    template <typename Application> 
    requires std::derived_from<Application, application::Application> && std::default_initializable<Application>
    static std::shared_ptr<Entrypoint> create(int argc, char** argv) {
        return std::make_unique<Entrypoint>(
            argc, argv, std::make_unique<Application>());
    }
    // clang-format on

    explicit Entrypoint(int argc, char** argv, std::unique_ptr<Application> application);

    int start();
    void onSignal(int sig) override;

private:
    void loopStep();
    void loadConfig();

    std::unique_ptr<Application> m_application;

    int m_argc;
    char** m_argv;
};
}

#define SL_ENTRYPOINT(App)                                                    \
    int main(int argc, char** argv) {                                         \
        return sl::application::Entrypoint::create<App>(argc, argv)->start(); \
    }
