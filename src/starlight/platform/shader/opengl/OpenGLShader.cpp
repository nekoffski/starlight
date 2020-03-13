#include <starlight/platform/shader/opengl/OpenGLShader.h>

#include <glad/glad.h>

namespace starl::platform::shader::opengl {

OpenGLShader::OpenGLShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath)
    : m_vertexPath(std::move(vertexPath))
    , m_fragmentPath(std::move(fragmentPath))
    , m_geometryPath(std::move(geometryPath)) {
}

void OpenGLShader::enable() {
    glUseProgram(m_shaderProgram);
}

void OpenGLShader::disable() {
    glUseProgram(0u);
}

OpenGLShader::~OpenGLShader() {
    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
    }
}
}