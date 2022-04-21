#pragma once

#include <kc/core/Log.h>
#include <kc/event/EventListener.h>

#include <kc/core/Singleton.hpp>
#include <memory>

#include "Keyboard.h"
#include "Macros.h"
#include "Mouse.h"
#include "sl/event/Event.h"
#include "sl/math/Vector.hpp"
#include "sl/platform/glfw/GlfwKeys.h"

namespace sl::core {

class InputManager : public kc::core::Singleton<InputManager>, public kc::event::EventListener {
   public:
    explicit InputManager() : EventListener("InputManager") {}

    void handleEvents(const kc::event::EventProvider& eventProvider) override {
        auto events = eventProvider.getByCategories<sl::event::CoreCategory>();

        for (auto& event : events)
            if (event->is<sl::event::ChangeViewportEvent>()) {
                m_viewport = event->asView<sl::event::ChangeViewportEvent>()->viewport;
                LOG_TRACE("Setting viewport for input manager");
            }
    }

    void setMouse(Mouse* mouse);
    void setKeyboard(Keyboard* keyboard);

    void update();

    bool isMouseButtonPressed(int buttonCode) const;

    math::Vec2 getMousePosition() const;
    math::Vec2 getMousePositonDelta() const;

    double getScrollDelta() const;

    bool isKeyPressed(int keyCode) const;

   private:
    Keyboard* m_keyboard;
    Mouse* m_mouse;
    gfx::Viewport m_viewport;
};

}  // namespace sl::core
