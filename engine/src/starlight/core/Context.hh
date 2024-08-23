#pragma once

#include <string>

#include "window/WindowManager.h"
#include "window/Vendor.h"

#include "event/Event.h"
#include "TimeManager.h"
#include "Config.h"
#include "Core.h"

namespace sl {

class Context : public NonCopyable {
    struct LoggerInitializator {
        explicit LoggerInitializator(const std::string& applicationName);
    };

public:
    explicit Context(const std::string& applicationName = "starlight");

    template <typename C>
    requires Callable<C, void, float>
    void beginFrame(C&& callback) {
        callback(beginFrame());
        endFrame();
    }

    Window& getWindow();
    Config& getConfig();

private:
    float beginFrame();
    void endFrame();

    LoggerInitializator m_loggerInitializator;

    Config m_config;
    EventManager m_eventManager;

    WindowVendor m_window;
    WindowManager m_windowManager;
    TimeManager m_timeManager;
};

}  // namespace sl