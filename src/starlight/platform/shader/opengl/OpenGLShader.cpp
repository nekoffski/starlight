#include <starlight/platform/shader/opengl/OpenGLShader.h>

#include <glad/glad.h>

namespace starl::platform::shader::opengl {

void OpenGLShader::enable() {
    glUseProgram(m_shaderProgram);
}

void OpenGLShader::disable() {
    glUseProgram(0);
}
}