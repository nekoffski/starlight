#pragma once

#include <kc/core/Singleton.hpp>

#include "Window.h"

namespace nova::core {

class WindowManager : public kc::core::Singleton<WindowManager> {
   public:
    explicit WindowManager(Window* window) : m_window(window) {}

   private:
    Window* m_window;
};

}  // namespace nova::core
