#include "Window.h"

#include "glfw/GLFWWindow.h"
#include "starlight/core/log/Logger.h"
#include "starlight/platform/PlatformDetector.h"

namespace sl::platform::window {

std::unique_ptr<Window> Window::create(WindowParams windowParams) {
#ifdef STARL_USE_OPENGL
    SL_INFO("instancing glfw window");
    return std::make_unique<glfw::GLFWWindow>(windowParams);
#endif
}
} // namespace sl::platform::window