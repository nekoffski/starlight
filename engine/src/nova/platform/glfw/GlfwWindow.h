#pragma once

#include <GLFW/glfw3.h>

#include <kc/core/Log.h>

#include "nova/core/Window.h"

namespace nova::platform::glfw {

class GlfwWindow : public core::Window {
   public:
    std::string_view getVendor() const override { return "GLFW3"; }

    explicit GlfwWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_windowHandle = glfwCreateWindow(1600, 900, "nova-engine", nullptr, nullptr);

        glfwMakeContextCurrent(m_windowHandle);

        static auto onWindowClose = []([[maybe_unused]] GLFWwindow*) -> void {
            LOG_INFO("Window received close request");
        };

        glfwSetWindowCloseCallback(m_windowHandle, onWindowClose);
    }

    void update() override { glfwPollEvents(); }

    void swapBuffers() override { glfwSwapBuffers(m_windowHandle); }

    math::Size2i getSize() const override {
        math::Size2i size;
        glfwGetWindowSize(m_windowHandle, &size.width, &size.height);

        return size;
    }

   private:
    GLFWwindow* m_windowHandle;
};

}  // namespace nova::platform::glfw