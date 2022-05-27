#pragma once

#include <concepts>

#include <kc/core/Log.h>

#include "nova/core/Window.h"
#include "nova/gfx/RendererBacked.h"

// TODO: pass by CMake
#ifndef USE_GLFW
#define USE_GLFW
#endif

#ifndef USE_VULKAN
#define USE_VULKAN
#endif

#ifdef USE_GLFW
#include "glfw/GlfwWindow.h"
#define WINDOW glfw::GlfwWindow
#else
#error "Window platform not specified"
#endif

#ifdef USE_VULKAN
#include "vulkan/VulkanRendererBackend.h"
#define RENDERER_BACKEND vulkan::VulkanRendererBackend
#endif

namespace nova::platform {

template <class T>
concept Window = std::is_base_of_v<core::Window, T>;

template <class T>
concept RendererBackend = std::is_base_of_v<gfx::RendererBackend, T>;

struct Platform {
    core::Window* window;
    gfx::RendererBackend* rendererBackend;
};

template <Window Window = WINDOW, RendererBackend RendererBackend = RENDERER_BACKEND>
class PlatformProvider {
   public:
    explicit PlatformProvider() : m_rendererBackend(m_window) {
        LOG_INFO("Window API vendor: {}", m_window.getVendor());
    }

    Platform getPlatform() {
        return Platform{.window = &m_window, .rendererBackend = &m_rendererBackend};
    }

   private:
    Window m_window;
    RendererBackend m_rendererBackend;
};

}  // namespace nova::platform
