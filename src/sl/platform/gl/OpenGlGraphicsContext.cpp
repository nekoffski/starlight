#include <glad/glad.h> // required by opengl to be included first

#include "OpenGlGraphicsContext.h"

#include "sl/core/Errors.hpp"
#include "sl/core/Logger.h"
#include "sl/core/Profiler.h"

#include "sl/utils/Globals.h"

static void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    SL_DEBUG("GL CALLBACK: {} type = {}, severity = {}, message = {}\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

namespace sl::platform::gl {

OpenGlGraphicsContext::OpenGlGraphicsContext(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {
    SL_INFO("setting context");
    glfwMakeContextCurrent(m_windowHandle);

    SL_INFO("loading glad memory proc");
    if (auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); status <= 0)
        throw core::GraphicsError { core::ErrorCode::CouldNotLoadGraphicsApi };

    // TODO: make it configurable
    // glEnable(GL_DEBUG_OUTPUT);
    auto& info = GLOBALS().info;

    info.gpuApiVendor = fmt::format("{}", glGetString(GL_VENDOR));
    info.gpuApiRelease = fmt::format("{}", glGetString(GL_VERSION));
    info.rendererName = fmt::format("{}", glGetString(GL_RENDERER));
    info.shadingLanguageVersion = fmt::format("{}", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glDebugMessageCallback(messageCallback, 0);
}

void OpenGlGraphicsContext::setViewport(sl::gfx::ViewFrustum::Viewport viewport) {
    glViewport(viewport.beginX, viewport.beginY, viewport.width, viewport.height);
}

void OpenGlGraphicsContext::swapBuffers() {
    SL_PROFILE_FUNCTION();
    glfwSwapBuffers(m_windowHandle);
}

void OpenGlGraphicsContext::clearBuffers(unsigned buffersToClear) {
    glClear(buffersToClear);
}

} // namespace sl::gfx
