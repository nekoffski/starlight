#include "OpenGlRenderApi.h"

#include <glad/glad.h>

#include "sl/core/Logger.h"

namespace sl::platform::gfx {

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
