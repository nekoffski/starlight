#pragma once

#include <functional>
#include <memory>

#include "sl/math/Vector.hpp"

namespace sl::core {

class Window {
public:
    struct Size {
        int width;
        int height;
    };

    using ResizeCallback = void (*)(void*, int, int);

    explicit Window(Size windowSize, const std::string& title)
        : m_defaultWindowSize(std::move(windowSize))
        , m_title(title) { }

    virtual ~Window() = default;

    virtual void setResizeCallback(ResizeCallback) = 0;

    virtual void init() = 0;
    virtual void update(float) = 0;

    virtual bool getShouldClose() const = 0;
    virtual void setShouldClose(bool) = 0;

    virtual void changeCursorState(bool) = 0;
    virtual void enableCursor() = 0;
    virtual void disableCursor() = 0;

    virtual void makeContextCurrent() = 0;
    virtual void swapBuffers() = 0;

    virtual void* getHandle() const = 0;

    virtual Size getSize() const = 0;

protected:
    Size m_defaultWindowSize;
    std::string m_title;
};
}
