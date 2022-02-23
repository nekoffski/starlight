#pragma once

#include <functional>
#include <memory>

#include "sl/math/Size2D.h"

namespace sl::core {

class Window {
   public:
    using ResizeCallback = void (*)(void*, int, int);

    explicit Window(math::Size2D windowSize, const std::string& title)
        : m_defaultWindowSize(std::move(windowSize)), m_title(title) {}

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

    virtual math::Size2D getSize() const = 0;

   protected:
    math::Size2D m_defaultWindowSize;
    std::string m_title;
};
}  // namespace sl::core
