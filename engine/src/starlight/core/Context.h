#pragma once

#include "window/WindowManager.h"
#include "window/Vendor.h"

#include "event/Event.h"
#include "TimeManager.h"
#include "Config.h"

namespace sl {

class Context {
   public:
    explicit Context();

    Window* getWindow();

    float beginFrame();
    void endFrame();

    Config* getConfig();

   private:
    Config m_config;
    EventManager m_eventManager;

    WindowVendor m_window;
    WindowManager m_windowManager;
    TimeManager m_timeManager;
};

}  // namespace sl