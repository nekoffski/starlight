#include <glad/glad.h> // required by opengl to be included first

#include "OpenGlGraphicsContext.h"

#include "sl/core/Logger.h"
#include "sl/core/Profiler.h"
#include "sl/core/error/Errors.hpp"

static void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    SL_DEBUG("GL CALLBACK: {} type = {}, severity = {}, message = {}\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

namespace sl::platform::gpu {

OpenGlGraphicsContext::OpenGlGraphicsContext(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {}

void OpenGlGraphicsContext::init() {
    SL_INFO("setting context");
    glfwMakeContextCurrent(m_windowHandle);

    SL_INFO("loading glad memory proc");
    if (auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); status <= 0)
        throw core::error::GraphicsError{ core::error::ErrorCode::CouldNotLoadGraphicsApi };

    // TODO: make it configurable
    //glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(messageCallback, 0);
}

void OpenGlGraphicsContext::setViewport(unsigned w, unsigned h) {
    glViewport(0, 0, w, h);
}

void OpenGlGraphicsContext::swapBuffers() {
    SL_PROFILE_FUNCTION();
    glfwSwapBuffers(m_windowHandle);
}

void OpenGlGraphicsContext::clearBuffers(unsigned buffersToClear) {
    glClear(buffersToClear);
}

} // namespace sl::graphics
