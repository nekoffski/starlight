#pragma once

#include <string>

#include "window/WindowManager.h"
#include "window/Vendor.h"

#include "event/Event.h"
#include "utils/TimeManager.h"
#include "Config.h"

namespace sl {

class Context {
    struct LoggerInitializator {
        explicit LoggerInitializator(const std::string& applicationName);
    };

   public:
    explicit Context(const std::string& applicationName = "starlight");

    Window* getWindow();

    float beginFrame();
    void endFrame();

    Config* getConfig();

   private:
    LoggerInitializator m_loggerInitializator;

    Config m_config;
    EventManager m_eventManager;

    WindowVendor m_window;
    WindowManager m_windowManager;
    TimeManager m_timeManager;
};

}  // namespace sl