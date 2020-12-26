#include "GlfwWindow.h"

#include <optional>

#include <GLFW/glfw3.h>

#include "sl/core/Logger.h"

namespace sl::platform::window {

// TODO: this cannot look like this
static std::optional<core::Window::ResizeCallback> gResizeCallback;

void executeResizeCallback(GLFWwindow* w, int width, int height) {
    if (gResizeCallback.has_value()) {
        auto windowSize = (core::Window::Size*)glfwGetWindowUserPointer(w);

        windowSize->width = width;
        windowSize->height = height;

        (*gResizeCallback)(width, height);
    }
}

GlfwWindow::~GlfwWindow() {
    if (m_windowHandle != nullptr) {
        SL_INFO("destroying");
        glfwDestroyWindow(m_windowHandle);
    }
}

void GlfwWindow::init() {
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
    m_windowHandle = glfwCreateWindow(m_windowSize.width, m_windowSize.height,
        m_title.c_str(), nullptr, nullptr);

    glfwSetWindowUserPointer(m_windowHandle, (void*)&m_windowSize);

    if (m_windowHandle == nullptr) {
        SL_ERROR("could not raw window instance");
        throw PlatformException(ErrorCode::COULD_NOT_CREATE_WINDOW_INSTANCE, "Could not create GLFW window instance");
    }
}

void GlfwWindow::setResizeCallback(core::Window::ResizeCallback resizeCallback) {
    gResizeCallback = resizeCallback;
    glfwSetWindowSizeCallback(m_windowHandle, executeResizeCallback);
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

void GlfwWindow::update(float dtime) {
    glfwPollEvents();
}
}
