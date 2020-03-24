#include <starlight/platform/window/Window.h>

#include <starlight/platform/PlatformDetector.h>

#include <starlight/platform/window/glfw/GLFWWindow.h>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("Window");

namespace starl::platform::window {

std::unique_ptr<Window> Window::create(WindowParams windowParams) {
#ifdef STARL_USE_OPENGL
    logger->info("instancing glfw window");
    return std::make_unique<glfw::GLFWWindow>(windowParams);
#endif
}
} // namespace starl::platform::window