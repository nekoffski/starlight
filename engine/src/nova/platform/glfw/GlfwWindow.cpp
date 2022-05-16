#include "GlfwWindow.h"

#include <GLFW/glfw3.h>
#include <kc/core/Log.h>

#include "nova/event/Event.h"
#include "nova/event/Quit.h"

namespace nova::platform::glfw {

#define TO_GLFW_PTR(ptr) static_cast<GLFWwindow*>(ptr)

GlfwWindow::GlfwWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_windowHandle = glfwCreateWindow(1600, 900, "nova-engine", nullptr, nullptr);

    glfwMakeContextCurrent(TO_GLFW_PTR(m_windowHandle));

    static auto onWindowClose = []([[maybe_unused]] GLFWwindow*) -> void {
        LOG_INFO("Window received close request");

        using namespace event;
        EventManager::get().emitEvent<QuitEvent>("Quit button pressed");
    };

    glfwSetWindowCloseCallback(TO_GLFW_PTR(m_windowHandle), onWindowClose);
}

void GlfwWindow::update() { glfwPollEvents(); }

void GlfwWindow::swapBuffers() { glfwSwapBuffers(TO_GLFW_PTR(m_windowHandle)); }

std::string_view GlfwWindow::getVendor() const { return "GLFW3"; }

math::Size2i GlfwWindow::getSize() const {
    math::Size2i size;
    glfwGetWindowSize(TO_GLFW_PTR(m_windowHandle), &size.width, &size.height);

    return size;
}

}  // namespace nova::platform::glfw