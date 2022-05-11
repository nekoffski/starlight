#pragma once

#include <concepts>

#include <kc/core/Log.h>

#include "nova/core/Window.h"

// TODO: pass by CMake
#ifndef USE_GLFW
#define USE_GLFW
#endif

#ifdef USE_GLFW
#include "glfw/GlfwWindow.h"
#define WINDOW glfw::GlfwWindow
#else
#error "Window platform not specified"
#endif

namespace nova::platform {

template <class T>
concept Window = std::is_base_of_v<core::Window, T>;

struct Platform {
    core::Window* window;
};

template <Window Window = WINDOW> class PlatformProvider {
   public:
    explicit PlatformProvider() { LOG_INFO("Window API vendor: {}", m_window.getVendor()); }

    Platform getPlatform() { return Platform{.window = &m_window}; }

   private:
    Window m_window;
};

}  // namespace nova::platform