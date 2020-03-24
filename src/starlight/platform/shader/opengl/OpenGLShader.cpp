#include <starlight/platform/shader/opengl/OpenGLShader.h>

#include <glad/glad.h>

#include <starlight/math/Matrix.hpp>
#include <starlight/math/Utils.hpp>

#include <iostream>

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

void OpenGLShader::setUniform(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void OpenGLShader::setUniform(const std::string& name, int value) {
    glUniform1f(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void OpenGLShader::setUniform(const std::string& name, unsigned int value) {
    glUniform1ui(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void OpenGLShader::setUniform(const std::string& name, math::Mat3 value) {
    glUniformMatrix3fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_FALSE, math::valuePtr(value));
}

void OpenGLShader::setUniform(const std::string& name, math::Mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_FALSE, math::valuePtr(value));
}

void OpenGLShader::setUniform(const std::string& name, math::Vec4 value) {
    glUniform4f(glGetUniformLocation(m_shaderProgram, name.c_str()), value.x, value.y, value.z, value.w);
}

void OpenGLShader::setUniform(const std::string& name, math::Vec3 value) {
    glUniform3f(glGetUniformLocation(m_shaderProgram, name.c_str()), value.x, value.y, value.z);
}
}