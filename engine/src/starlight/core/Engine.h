#pragma once

#include <kc/sig/SignalHandler.h>

#include "fwd.h"

#include "starlight/platform/Platform.h"
#include "starlight/event/Event.h"
#include "starlight/gfx/RendererFrontend.h"
#include "starlight/gfx/EulerCamera.h"
#include "starlight/gfx/FirstPersonCamera.h"
#include "starlight/gfx/TextureManager.h"
#include "starlight/gfx/MaterialManager.h"
#include "starlight/gfx/GeometryManager.h"
#include "TimeManager.h"
#include "WindowManager.h"

namespace sl::core {

class Engine : public kc::sig::SignalHandler, public event::EventObserver {
   public:
    explicit Engine(const platform::Platform& platform);
    ~Engine();

    void run(Application& application);

   private:
    void onSignal(int signal) override;

    void update(Application& application, float deltaTime);
    void render(Application& application, float deltaTime);

    void onEvent(event::EventWrapper& event) override;

    bool m_shouldStop;

    platform::Platform m_platform;

    TimeManager m_timeManager;
    event::EventManager m_eventManager;
    WindowManager m_windowManager;
    gfx::TextureManager m_textureManager;
    gfx::MaterialManager m_materialManager;
    gfx::GeometryManager m_geometryManager;

    gfx::RendererFrontend m_rendererFrontend;
    gfx::EulerCamera m_eulerCamera;
    gfx::FirstPersonCamera m_firstPersonCamera;

    gfx::Camera* m_currentCamera;
};

}  // namespace sl::core