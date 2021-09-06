#include "OpenGlShaderCompiler.h"

#include <cstring>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include <kc/core/Log.h>

#include "sl/core/Errors.hpp"

#include "OpenGlShader.h"

constexpr int InfoBufferSize = 1024;

using namespace sl::core;

namespace sl::platform::gl {

void OpenGlShaderCompiler::compile(sl::gfx::Shader& shader) {
    compile(dynamic_cast<OpenGlShader&>(shader));
}

void OpenGlShaderCompiler::compile(OpenGlShader& shader) {
    LOG_DEBUG("compiling shader: \n{},\n{},\n{}", shader.m_fragmentPath, shader.m_vertexPath, shader.m_geomPath);

    GLuint& shaderProgramId = shader.m_shaderProgram;
    shaderProgramId = glCreateProgram();

    LOG_DEBUG("compiling vertex shader");
    auto vertexShaderId = compileShader(shader.m_vertexPath, GL_VERTEX_SHADER);
    LOG_DEBUG("compiling fragment shader");
    auto fragmentShaderId = compileShader(shader.m_fragmentPath, GL_FRAGMENT_SHADER);

    bool hasGeometryShader = shader.m_geomPath != "";
    std::optional<unsigned int> geometryShaderId;

    if (hasGeometryShader) {
        LOG_DEBUG("compiling geometry shader");
        geometryShaderId = compileShader(shader.m_geomPath, GL_GEOMETRY_SHADER);

        glAttachShader(shaderProgramId, geometryShaderId.value());
    }

    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);

    LOG_DEBUG("linking shader");
    glLinkProgram(shaderProgramId);

    char infoBuffer[infoBufferSize];
    int linked = 0;
    std::memset(infoBuffer, 0, infoBufferSize);

    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &linked);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    if (geometryShaderId.has_value())
        glDeleteShader(geometryShaderId.value());

    if (!linked) {
        glGetProgramInfoLog(shaderProgramId, infoBufferSize, nullptr, infoBuffer);
        LOG_ERROR("could not link: ", infoBuffer);
        throw core::ShaderError {};
    }
}

unsigned int OpenGlShaderCompiler::compileShader(const std::string& path, unsigned type) {
    char infoBuffer[infoBufferSize];
    int compiled = 0;

    std::fstream shaderSource(path);

    if (!shaderSource.good()) {
        LOG_ERROR("could not find source: {}", path);
        throw ShaderError {};
    }

    std::stringstream vertex_data;
    vertex_data << shaderSource.rdbuf();

    std::string str_vertex_data = vertex_data.str();

    shaderSource.close();

    int shader = glCreateShader(type);

    const char* shaderData = str_vertex_data.c_str();

    glShaderSource(shader, 1, &shaderData, nullptr);
    glCompileShader(shader);

    std::memset(infoBuffer, 0, infoBufferSize);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        glGetShaderInfoLog(shader, infoBufferSize, nullptr, infoBuffer);
        LOG_ERROR("could not compile: {}", infoBuffer);
        throw ShaderError {};
    }

    return shader;
}
}
