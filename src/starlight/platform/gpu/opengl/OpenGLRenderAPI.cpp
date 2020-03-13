#include <starlight/platform/gpu/opengl/OpenGLRenderAPI.h>

#include <glad/glad.h>

namespace starl::platform::gpu::opengl {

void OpenGLRenderAPI::drawArrays(unsigned mode, unsigned begin, unsigned end) {
    glDrawArrays(mode, begin, end);
}

void OpenGLRenderAPI::clearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}
}