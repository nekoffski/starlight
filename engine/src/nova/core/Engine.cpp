#include "Engine.h"

#include <kc/core/Log.h>
#include <kc/sig/Signal.h>

#include <unistd.h>

#include "Application.h"
#include "Core.hpp"

#include "nova/event/Quit.h"

namespace nova::core {

Engine::Engine(const platform::Platform& platform) : m_shouldStop(false), m_platform(platform) {
    LOG_TRACE("Setting up signals");
    kc::sig::setupSignalHandler(this);

    LOG_TRACE("Registering Engine as an event observer");
    m_eventManager.registerObserver(this);
}

void Engine::run(Application& application) {
    LOG_TRACE("Starting main engine loop");

    const float deltaTime = 0.01f;  // mock

    while (not m_shouldStop && application.isRunning()) {
        update(application, deltaTime);
        render(application);
    }

    LOG_TRACE("Main engine loop ended");
}

void Engine::update(Application& application, float deltaTime) {
    m_platform.window->update();
    application.update(deltaTime);
}

void Engine::render(Application& application) { m_platform.window->swapBuffers(); }

void Engine::onSignal(int signal) {
    LOG_WARN("Received signal {}, stopping engine", signal);
    m_shouldStop = true;
}

void Engine::onEvent(event::EventWrapper& event) {
    if (event.is<event::QuitEvent>()) {
        LOG_INFO("Received QuitEvent with reason: {}", event.get<event::QuitEvent>().reason);
        m_shouldStop = true;
    }
}

}  // namespace nova::core