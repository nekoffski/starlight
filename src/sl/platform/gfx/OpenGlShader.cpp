#include "OpenGlShader.h"

#include <glad/glad.h>

#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"

namespace sl::platform::gfx {

OpenGlShader::OpenGlShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geomPath)
    : m_vertexPath(vertexPath)
    , m_fragmentPath(fragmentPath)
    , m_geomPath(geomPath) {
}

void OpenGlShader::enable() {
    glUseProgram(m_shaderProgram);
}

void OpenGlShader::disable() {
    glUseProgram(0u);
}

OpenGlShader::~OpenGlShader() {
    if (m_shaderProgram)
        glDeleteProgram(m_shaderProgram);
}

void OpenGlShader::setUniform(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void OpenGlShader::setUniform(const std::string& name, int value) {
    glUniform1f(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void OpenGlShader::setUniform(const std::string& name, unsigned int value) {
    glUniform1ui(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void OpenGlShader::setUniform(const std::string& name, math::Mat3 value) {
    glUniformMatrix3fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_FALSE, math::valuePtr(value));
}

void OpenGlShader::setUniform(const std::string& name, math::Mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_FALSE, math::valuePtr(value));
}

void OpenGlShader::setUniform(const std::string& name, math::Vec4 value) {
    glUniform4f(glGetUniformLocation(m_shaderProgram, name.c_str()), value.x, value.y, value.z, value.w);
}

void OpenGlShader::setUniform(const std::string& name, math::Vec3 value) {
    glUniform3f(glGetUniformLocation(m_shaderProgram, name.c_str()), value.x, value.y, value.z);
}
}
