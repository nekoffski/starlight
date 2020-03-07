#pragma once

#include <memory>

#include <starlight/platform/window/WindowParams.h>

namespace starl::platform::window {

class Window {
public:
    static std::unique_ptr<Window> create(WindowParams windowParams);

    explicit Window(WindowParams windowParams)
        : m_windowParams(std::move(windowParams)) {}

    virtual ~Window() = default;

    virtual void init() = 0;
    virtual void update(float) = 0;

    virtual bool getShouldClose() const = 0;
    virtual void setShouldClose(bool) = 0;

    virtual void* getHandle() const = 0;

    const WindowParams& getParams() const {
        return m_windowParams;
    }

protected:
    WindowParams m_windowParams;
};
}
