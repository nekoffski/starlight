#include <glad/glad.h> // required by opengl to be included first

#include "OpenGLGraphicsContext.h"

#include "starlight/core/log/Logger.h"

namespace sl::platform::gpu::opengl {

OpenGLGraphicsContext::OpenGLGraphicsContext(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {}

void OpenGLGraphicsContext::init() {
    SL_INFO("setting context");
    glfwMakeContextCurrent(m_windowHandle);

    SL_INFO("loading glad memory proc");
    if (auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); status <= 0)
        throw PlatformException(ErrorCode::COULD_NOT_LOAD_GRAPHICS_HANDLE, "Could not load glad proc addr", status);
}

void OpenGLGraphicsContext::swapBuffers() {
    glfwSwapBuffers(m_windowHandle);
}

void OpenGLGraphicsContext::clearBuffers(unsigned buffersToClear) {
    glClear(buffersToClear);
}

} // namespace sl::platform::gpu::opengl
