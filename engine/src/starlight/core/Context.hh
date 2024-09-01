#pragma once

#include <string>

#include "window/WindowImpl.hh"
#include "window/Window.hh"
#include "window/Vendor.hh"
#include "window/Input.hh"

#include "event/EventBroker.hh"
#include "Clock.hh"
#include "Config.hh"
#include "Core.hh"

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
    EventBroker m_eventBroker;

    WindowVendor m_windowImpl;
    Window m_window;
    Input m_input;
    Clock m_clock;
};

}  // namespace sl