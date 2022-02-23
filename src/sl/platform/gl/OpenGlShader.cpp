#include "OpenGlShader.h"

#include <glad/glad.h>

#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"

namespace sl::platform::gl {

OpenGlShader::OpenGlShader(const std::string& vertexPath, const std::string& fragmentPath,
                           const std::string& geomPath)
    : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath), m_geomPath(geomPath) {}

void OpenGlShader::enable() { glUseProgram(m_shaderProgram); }

void OpenGlShader::disable() { glUseProgram(0u); }

OpenGlShader::~OpenGlShader() {
    if (m_shaderProgram) glDeleteProgram(m_shaderProgram);
}

void OpenGlShader::setUniform(const std::string& name, float value) {
    glProgramUniform1f(m_shaderProgram, glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void OpenGlShader::setUniform(const std::string& name, int value) {
    glProgramUniform1i(m_shaderProgram, glGetUniformLocation(m_shaderProgram, name.c_str()), value);
}

void OpenGlShader::setUniform(const std::string& name, unsigned int value) {
    glProgramUniform1ui(m_shaderProgram, glGetUniformLocation(m_shaderProgram, name.c_str()),
                        value);
}

void OpenGlShader::setUniform(const std::string& name, math::Mat3 value) {
    glProgramUniformMatrix3fv(m_shaderProgram, glGetUniformLocation(m_shaderProgram, name.c_str()),
                              1, GL_FALSE, math::valuePtr(value));
}

void OpenGlShader::setUniform(const std::string& name, math::Mat4 value) {
    glProgramUniformMatrix4fv(m_shaderProgram, glGetUniformLocation(m_shaderProgram, name.c_str()),
                              1, GL_FALSE, math::valuePtr(value));
}

void OpenGlShader::setUniform(const std::string& name, math::Vec4 value) {
    glProgramUniform4f(m_shaderProgram, glGetUniformLocation(m_shaderProgram, name.c_str()),
                       value.x, value.y, value.z, value.w);
}

void OpenGlShader::setUniform(const std::string& name, math::Vec3 value) {
    glProgramUniform3f(m_shaderProgram, glGetUniformLocation(m_shaderProgram, name.c_str()),
                       value.x, value.y, value.z);
}
}  // namespace sl::platform::gl
