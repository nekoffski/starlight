#pragma once

#include <kc/sig/SignalHandler.h>

#include "fwd.h"

#include "nova/platform/Platform.h"
#include "nova/event/Event.h"
#include "nova/gfx/RendererFrontend.h"
#include "WindowManager.h"

namespace nova::core {

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

    event::EventManager m_eventManager;
    WindowManager m_windowManager;

    gfx::RendererFrontend m_rendererFrontend;
};

}  // namespace nova::core