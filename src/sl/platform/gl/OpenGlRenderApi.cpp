#include <glad/glad.h>

// TODO: try to get rid of
#include <GLFW/glfw3.h>

#include "OpenGlRenderApi.h"

#include "sl/core/Errors.hpp"
#include "sl/utils/Globals.h"
#include <kc/core/Log.h>

static void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    LOG_DEBUG("GL CALLBACK: {} type = {}, severity = {}, message = {}\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

namespace sl::platform::gl {

void OpenGlRenderApi::init() {
    LOG_INFO("Loading glad memory proccess");
    if (auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); status <= 0)
        throw core::GraphicsError {};

    // TODO: make it configurable
    // glEnable(GL_DEBUG_OUTPUT);

    auto& info = GLOBALS().info;

    info.gpuApiVendor = fmt::format("{}", glGetString(GL_VENDOR));
    info.gpuApiRelease = fmt::format("{}", glGetString(GL_VERSION));
    info.rendererName = fmt::format("{}", glGetString(GL_RENDERER));
    info.shadingLanguageVersion = fmt::format("{}", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glDebugMessageCallback(messageCallback, 0);
}

void OpenGlRenderApi::setViewport(const gfx::ViewFrustum::Viewport& viewport) {
    glViewport(viewport.beginX, viewport.beginY, viewport.width, viewport.height);
}

void OpenGlRenderApi::clearBuffers(unsigned int buffers) {
    glClear(buffers);
}

void OpenGlRenderApi::setCullFace(unsigned int face) {
    glCullFace(face);
}

void OpenGlRenderApi::drawArrays(unsigned mode, unsigned begin, unsigned end) {
    glDrawArrays(mode, begin, end);
}

void OpenGlRenderApi::drawElements(unsigned mode, unsigned count, unsigned type) {
    glDrawElements(mode, count, type, 0);
}

void OpenGlRenderApi::clearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void OpenGlRenderApi::depthMask(bool value) {
    glDepthMask(value);
}

void OpenGlRenderApi::setPolygonMode(unsigned value) {
    glPolygonMode(GL_FRONT_AND_BACK, value);
}

void OpenGlRenderApi::setDepthFunc(unsigned func) {
    glDepthFunc(func);
}

void OpenGlRenderApi::setBlendFunc(unsigned sfactor, unsigned dfactor) {
    glBlendFunc(sfactor, dfactor);
}

void OpenGlRenderApi::enable(unsigned mode) {
    glEnable(mode);
}

void OpenGlRenderApi::disable(unsigned mode) {
    glDisable(mode);
}
}
