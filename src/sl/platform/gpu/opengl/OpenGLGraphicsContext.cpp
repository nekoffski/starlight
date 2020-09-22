#include <glad/glad.h> // required by opengl to be included first

#include "OpenGLGraphicsContext.h"

#include "sl/core/log/Logger.h"
#include "sl/core/perf/Profiler.h"

static void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    SL_DEBUG("GL CALLBACK: {} type = {}, severity = {}, message = {}\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

namespace sl::platform::gpu::opengl {

OpenGLGraphicsContext::OpenGLGraphicsContext(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {}

void OpenGLGraphicsContext::init() {
    SL_INFO("setting context");
    glfwMakeContextCurrent(m_windowHandle);

    SL_INFO("loading glad memory proc");
    if (auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); status <= 0)
        throw PlatformException(ErrorCode::COULD_NOT_LOAD_GRAPHICS_HANDLE, "Could not load glad proc addr", status);

    // TODO: make it configurable
    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(messageCallback, 0);
}

void OpenGLGraphicsContext::swapBuffers() {
    PRF_PROFILE_FUNCTION();
    glfwSwapBuffers(m_windowHandle);
}

void OpenGLGraphicsContext::clearBuffers(unsigned buffersToClear) {
    glClear(buffersToClear);
}

} // namespace sl::platform::gpu::opengl
