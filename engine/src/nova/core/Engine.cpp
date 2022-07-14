#include "Engine.h"

#include <kc/core/Log.h>
#include <kc/sig/Signal.h>

#include <unistd.h>

#include "Application.h"
#include "Core.hpp"

#include "nova/event/Quit.h"

namespace nova::core {

Engine::Engine(const platform::Platform& platform)
    : m_shouldStop(false)
    , m_platform(platform)
    , m_windowManager(m_platform.window)
    , m_rendererFrontend(m_platform.rendererBackend) {
    LOG_TRACE("Setting up signals");
    kc::sig::setupSignalHandler(this);

    LOG_TRACE("Registering Engine as an event observer");
    m_eventManager.registerObserver(this);
}

Engine::~Engine() { kc::sig::resetSignalHandler(); }

void Engine::run(Application& application) {
    LOG_TRACE("Starting main engine loop");

    const float deltaTime = 0.01f;  // mock

    while (not m_shouldStop && application.isRunning()) {
        update(application, deltaTime);
        render(application, deltaTime);
    }

    LOG_TRACE("Main engine loop ended");
}

void Engine::update(Application& application, float deltaTime) {
    m_windowManager.update();

    application.update(deltaTime);
}

void Engine::render(Application& application, float deltaTime) {
    gfx::RenderPacket packet{};

    m_rendererFrontend.drawFrame(packet, deltaTime);
}

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