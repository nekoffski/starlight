#include <glad/glad.h>

#include <starlight/platform/gpu/opengl/OpenGLGraphicsContext.h>

namespace starl::platform::gpu::opengl {

OpenGLGraphicsContext::OpenGLGraphicsContext(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {}

void OpenGLGraphicsContext::init() {
    glfwMakeContextCurrent(m_windowHandle);

    if (auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); status <= 0) {
        throw PlatformException(ErrorCode::COULD_NOT_LOAD_GRAPHICS_HANDLE, "Could not load glad proc addr", status);
    }
}

void OpenGLGraphicsContext::swapBuffers() {
    glfwSwapBuffers(m_windowHandle);
}

void OpenGLGraphicsContext::clearBuffers() {
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace starl::platform::gpu::opengl
