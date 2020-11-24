#include "GLFWWindow.h"

#include "sl/core/log/Logger.h"
#include <GLFW/glfw3.h>

namespace sl::platform::window::glfw {

GLFWWindow::~GLFWWindow() {
    if (m_windowHandle != nullptr) {
        SL_INFO("destroying");
        glfwDestroyWindow(m_windowHandle);
    }
}

void GLFWWindow::init() {
    SL_INFO("initializing glfw");
    if (auto status = glfwInit(); status < 0) {
        SL_ERROR("could not initialize glfw: {]", status);
        throw PlatformException(ErrorCode::COULD_NOT_INIT_WINDOW_LIBRARY, "Could not load GLFW", status);
    }

    SL_INFO("setting window hints");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SL_INFO("creating raw window instance");
    m_windowHandle = glfwCreateWindow(m_windowParams.viewport.width, m_windowParams.viewport.height,
        m_windowParams.title.c_str(), nullptr, nullptr);

    if (m_windowHandle == nullptr) {
        SL_ERROR("could not raw window instance");
        throw PlatformException(ErrorCode::COULD_NOT_CREATE_WINDOW_INSTANCE, "Could not create GLFW window instance");
    }
}

bool GLFWWindow::getShouldClose() const {
    return glfwWindowShouldClose(m_windowHandle);
}

void GLFWWindow::setShouldClose(bool value) {
    glfwSetWindowShouldClose(m_windowHandle, value);
}

void GLFWWindow::enableCursor() {
    glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GLFWWindow::disableCursor() {
    glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GLFWWindow::update(float dtime) {
    glfwPollEvents();
}
}
