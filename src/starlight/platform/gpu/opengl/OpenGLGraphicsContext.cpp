#include <glad/glad.h>

#include <starlight/platform/gpu/opengl/OpenGLGraphicsContext.h>

namespace starl::platform::gpu::opengl {

OpenGLGraphicsContext::OpenGLGraphicsContext(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {}

void OpenGLGraphicsContext::init() {
    glfwMakeContextCurrent(m_windowHandle);

    if (auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); status <= 0) {
        throw PlatformException(ErrorCode::COULD_NOT_LOAD_GRAPHICS_HANDLE, status);
    }
}

void OpenGLGraphicsContext::swapBuffers() {
    glfwSwapBuffers(m_windowHandle);
}

} // namespace starl::platform::gpu::opengl
