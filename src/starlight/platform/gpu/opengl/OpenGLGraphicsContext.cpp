#include <glad/glad.h>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("OpenGLGraphicsContext");

#include <starlight/platform/gpu/opengl/OpenGLGraphicsContext.h>

namespace starl::platform::gpu::opengl {

OpenGLGraphicsContext::OpenGLGraphicsContext(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {}

void OpenGLGraphicsContext::init() {
    logger->info("setting context");
    glfwMakeContextCurrent(m_windowHandle);

    logger->info("loading glad memory proc");
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
