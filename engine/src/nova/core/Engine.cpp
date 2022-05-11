#include "Engine.h"

#include <kc/core/Log.h>
#include <kc/sig/Signal.h>

#include <unistd.h>

#include "Application.h"
#include "Core.hpp"

namespace nova::core {

Engine::Engine(const platform::Platform& platform) : m_shouldStop(false), m_platform(platform) {
    LOG_TRACE("Setting up signals");
    kc::sig::setupSignalHandler(this);
}

void Engine::run(Application& application) {
    LOG_TRACE("Starting main engine loop");

    while (not m_shouldStop && application.isRunning()) {
        loopStep(application);
    }

    LOG_TRACE("Main engine loop ended");
}

void Engine::loopStep(Application& application) {
    const float deltaTime = 0.01f;  // mock

    m_platform.window->update();
    m_platform.window->swapBuffers();

    application.update(deltaTime);
}

void Engine::onSignal(int signal) {
    LOG_WARN("Received signal {}, stopping engine", signal);
    m_shouldStop = true;
}

}  // namespace nova::core