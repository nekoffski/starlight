#pragma once

#include <kc/event/EventListener.h>

#include <kc/core/Singleton.hpp>
#include <memory>

#include "Window.h"
#include "sl/event/Event.h"
#include "sl/gfx/Viewport.h"

namespace sl::core {

class WindowManager : public kc::core::Singleton<WindowManager>, public kc::event::EventListener {
   public:
    explicit WindowManager() : EventListener("WindowManager") {}

    void handleEvents(const kc::event::EventProvider& eventProvider) override {
        auto events = eventProvider.getByCategories<sl::event::CoreCategory>();

        for (auto& event : events)
            if (event->is<sl::event::ChangeViewportEvent>())
                m_viewport = event->asView<sl::event::ChangeViewportEvent>()->viewport;
    }

    void setActiveWindow(Window* window) { m_window = window; }

    void enableCursor(bool shouldEnable) { m_window->changeCursorState(shouldEnable); }

    void enableCursor() { m_window->enableCursor(); }

    void disableCursor() { m_window->disableCursor(); }

    math::Size2D getSize() const { return m_window->getSize(); }

    void* getWindowHandle() const { return m_window->getHandle(); }

    const gfx::Viewport& getViewport() const { return m_viewport; }

   private:
    gfx::Viewport m_viewport;
    Window* m_window;
};

}  // namespace sl::core