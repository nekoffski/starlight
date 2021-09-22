#include "GlfwWindow.h"

#include <optional>

#include <GLFW/glfw3.h>
#include <kc/core/Log.h>
#include <kc/core/Profiler.h>

#include "sl/core/Errors.hpp"
#include "sl/event/Event.h"
#include "sl/event/EventManager.h"

namespace sl::platform::glfw {

GlfwWindow::~GlfwWindow() {
    if (m_windowHandle != nullptr) {
        LOG_INFO("Destroying glfw window");
        glfwDestroyWindow(m_windowHandle);
        glfwTerminate();
    }
}

void GlfwWindow::init() {
    LOG_INFO("Initializing glfw");
    if (auto status = glfwInit(); status < 0) {
        LOG_FATAL("could not initialize glfw: {}", status);
        throw core::WindowError {};
    }

    LOG_INFO("Setting up window hints");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback([](int errorCode, const char* message) {
        LOG_FATAL("GLFW ERROR {} - {}", errorCode, message);
    });

    LOG_INFO("Creating window's handle instance");
    m_windowHandle = glfwCreateWindow(m_defaultWindowSize.width, m_defaultWindowSize.height,
        m_title.c_str(), nullptr, nullptr);

    if (m_windowHandle == nullptr) {
        LOG_FATAL("could not create raw window instance");
        throw core::WindowError {};
    }

    glfwSetWindowCloseCallback(m_windowHandle, []([[maybe_unused]] GLFWwindow*) {
        LOG_DEBUG("Window's close button pressed, emitting QuitEvent");
        event::EventManager::get()->emit<event::QuitEvent>().toAll();
    });
}

void GlfwWindow::setResizeCallback(core::Window::ResizeCallback resizeCallback) {
    glfwSetWindowSizeCallback(m_windowHandle, (GLFWwindowsizefun)resizeCallback);
}

bool GlfwWindow::getShouldClose() const {
    return glfwWindowShouldClose(m_windowHandle);
}

void GlfwWindow::setShouldClose(bool value) {
    glfwSetWindowShouldClose(m_windowHandle, value);
}

void GlfwWindow::enableCursor() {
    glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GlfwWindow::disableCursor() {
    glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GlfwWindow::changeCursorState(bool enabled) {
    glfwSetInputMode(m_windowHandle, GLFW_CURSOR,
        GLFW_CURSOR_DISABLED - (GLFW_CURSOR_DISABLED - GLFW_CURSOR_NORMAL) * enabled);
}

void GlfwWindow::update(float dtime) {
    glfwPollEvents();
}

void GlfwWindow::makeContextCurrent() {
    glfwMakeContextCurrent(m_windowHandle);
}

void GlfwWindow::swapBuffers() {
    PROFILE_FUNCTION();
    glfwSwapBuffers(m_windowHandle);
}

void* GlfwWindow::getHandle() const {
    return static_cast<void*>(m_windowHandle);
}

math::Size2D GlfwWindow::getSize() const {
    int x, y;
    glfwGetWindowSize(m_windowHandle, &x, &y);

    return { x, y };
}

}
