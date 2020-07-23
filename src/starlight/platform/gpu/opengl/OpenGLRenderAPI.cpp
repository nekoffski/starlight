#include <starlight/platform/gpu/opengl/OpenGLRenderAPI.h>

#include <glad/glad.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::gpu::opengl {

void OpenGLRenderAPI::drawArrays(unsigned mode, unsigned begin, unsigned end) {
    glDrawArrays(GL_TRIANGLES, begin, end);
}

void OpenGLRenderAPI::drawElements(unsigned mode, unsigned count, unsigned type) {
    glDrawElements(mode, count, type, 0);
}

void OpenGLRenderAPI::clearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void OpenGLRenderAPI::enableDepthMask() {
    glDepthMask(GL_TRUE);
}

void OpenGLRenderAPI::disableDepthMask() {
    glDepthMask(GL_FALSE);
}

void OpenGLRenderAPI::setDepthFunc(unsigned func) {
    glDepthFunc(func);
}

void OpenGLRenderAPI::setBlendFunc(unsigned sfactor, unsigned dfactor) {
    glBlendFunc(sfactor, dfactor);
}

void OpenGLRenderAPI::enable(unsigned mode) {
    glEnable(mode);
}

void OpenGLRenderAPI::disable(unsigned mode) {
    glDisable(mode);
}
}