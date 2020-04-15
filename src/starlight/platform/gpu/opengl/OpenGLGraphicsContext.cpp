#include <glad/glad.h> // required by opengl to be included first

#include <starlight/platform/gpu/opengl/OpenGLGraphicsContext.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::gpu::opengl {

OpenGLGraphicsContext::OpenGLGraphicsContext(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {}

void OpenGLGraphicsContext::init() {
    LOG(INFO) << "setting context";
    glfwMakeContextCurrent(m_windowHandle);

    LOG(INFO) << "loading glad memory proc";
    if (auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); status <= 0) {
        throw PlatformException(ErrorCode::COULD_NOT_LOAD_GRAPHICS_HANDLE, "Could not load glad proc addr", status);
    }
}

void OpenGLGraphicsContext::swapBuffers() {
    glfwSwapBuffers(m_windowHandle);
}

void OpenGLGraphicsContext::clearBuffers(unsigned buffersToClear) {
    glClear(buffersToClear);
}

} // namespace starl::platform::gpu::opengl
