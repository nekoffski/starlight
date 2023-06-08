#include "Engine.h"

#include <kc/core/Log.h>
#include <kc/sig/Signal.h>

#include <kc/core/Clock.h>

#include <unistd.h>

#include "Application.h"
#include "Core.hpp"

// TODO: rename to GPUProxy
#include "starlight/gfx/ResourceProxy.h"

#include "starlight/event/Quit.h"

std::string_view texturesPath  = SL_ASSETS_TEXTURES_PATH;
std::string_view materialsPath = SL_ASSETS_MATERIALS_PATH;

#include "starlight/gfx/Geometry.h"

namespace sl::core {

static gfx::Geometry* testGeometry;

Engine::Engine(const platform::Platform& platform)
    : m_shouldStop(false)
    , m_platform(platform)
    , m_windowManager(m_platform.window)
    , m_textureManager(*m_platform.rendererBackend->getTextureLoader(), texturesPath)
    , m_materialManager(
          m_textureManager, gfx::ResourceProxy(*m_platform.rendererBackend), materialsPath
      )
    , m_geometryManager(gfx::ResourceProxy(*m_platform.rendererBackend))
    , m_rendererFrontend(m_platform.rendererBackend)
    , m_eulerCamera(gfx::EulerCamera::Properties{.target = math::Vec3f{0.0f}, .radius = 5.0f}) {
    LOG_DEBUG("Setting up signals");
    kc::sig::setupSignalHandler(this);

    LOG_DEBUG("Registering Engine as an event observer");
    m_eventManager.registerObserver(this);

    // m_currentCamera = &m_firstPersonCamera;
    m_currentCamera = &m_eulerCamera;
}

Engine::~Engine() { kc::sig::resetSignalHandler(); }

void Engine::run(Application& application) {
    LOG_DEBUG("Starting main engine loop");

    ASSERT(m_currentCamera != nullptr, "Camera is not set");

    testGeometry = m_geometryManager.getDefault();

    while (not m_shouldStop && application.isRunning()) {
        const float deltaTime = m_timeManager.getDeltaTime();

        update(application, deltaTime);
        render(application, deltaTime);

        m_timeManager.update();
    }

    LOG_DEBUG("Main engine loop ended");
}

void Engine::update(Application& application, float deltaTime) {
    m_windowManager.update();

    m_currentCamera->update(deltaTime);

    application.update(deltaTime);
}

void Engine::render(Application& application, float deltaTime) {
    gfx::RenderPacket packet{};

    gfx::GeometryRenderData testRender;

    testRender.geometry = testGeometry;
    testRender.model    = math::identityMatrix;

    std::vector<gfx::GeometryRenderData> geometries = {testRender};

    packet.geometries = geometries;

    m_rendererFrontend.drawFrame(packet, *m_currentCamera, deltaTime);
}

void Engine::onSignal(int signal) {
    LOG_WARN("Received signal {}, stopping engine", signal);
    // TODO: change to atomic
    m_shouldStop = true;
}

void Engine::onEvent(event::EventWrapper& event) {
    if (event.is<event::QuitEvent>()) {
        LOG_INFO("Received QuitEvent with reason: {}", event.get<event::QuitEvent>().reason);
        m_shouldStop = true;
    }
}

}  // namespace sl::core