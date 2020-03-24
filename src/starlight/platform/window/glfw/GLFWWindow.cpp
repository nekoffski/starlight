#include <starlight/platform/window/glfw/GLFWWindow.h>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("GLFWWindow");

namespace starl::platform::window::glfw {

GLFWWindow::~GLFWWindow() {
    if (m_windowHandle != nullptr) {
        logger->info("destroying");
        glfwDestroyWindow(m_windowHandle);
    }
}

void GLFWWindow::init() {
    logger->info("initializing glfw");
    if (auto status = glfwInit(); status < 0) {
        logger->error("could not initialize glfw: {}", status);
        throw PlatformException(ErrorCode::COULD_NOT_INIT_WINDOW_LIBRARY, "Could not load GLFW", status);
    }

    logger->info("setting window hints");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    logger->info("creating raw window instance");
    m_windowHandle = glfwCreateWindow(m_windowParams.viewport.width, m_windowParams.viewport.height,
        m_windowParams.title.c_str(), nullptr, nullptr);

    if (m_windowHandle == nullptr) {
        logger->error("could not raw window instance");
        throw PlatformException(ErrorCode::COULD_NOT_CREATE_WINDOW_INSTANCE, "Could not create GLFW window instance");
    }
}

bool GLFWWindow::getShouldClose() const {
    return glfwWindowShouldClose(m_windowHandle);
}

void GLFWWindow::setShouldClose(bool value) {
    glfwSetWindowShouldClose(m_windowHandle, value);
}

void GLFWWindow::update(float dtime) {
    glfwPollEvents();
}
}