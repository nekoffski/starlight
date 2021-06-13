#include "OpenGlShaderCompilerImpl.h"

#include <cstring>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include "OpenGlShader.h"
#include "sl/core/ErrorCode.h"
#include "sl/core/Errors.hpp"
#include "sl/core/Logger.h"

constexpr int InfoBufferSize = 1024;

using namespace sl::core;

namespace sl::platform::gfx {

void OpenGlShaderCompilerImpl::compile(sl::gfx::Shader& shader) {
    compileImpl(dynamic_cast<OpenGlShader&>(shader));
}

void OpenGlShaderCompilerImpl::compileImpl(OpenGlShader& shader) {
    SL_DEBUG("compiling shader: \n{},\n{},\n{}", shader.m_fragmentPath, shader.m_vertexPath, shader.m_geomPath);

    GLuint& shaderProgramId = shader.m_shaderProgram;
    shaderProgramId = glCreateProgram();

    SL_DEBUG("compiling vertex shader");
    auto vertexShaderId = compileShader(shader.m_vertexPath, GL_VERTEX_SHADER);
    SL_DEBUG("compiling fragment shader");
    auto fragmentShaderId = compileShader(shader.m_fragmentPath, GL_FRAGMENT_SHADER);

    bool hasGeometryShader = shader.m_geomPath != "";
    std::optional<unsigned int> geometryShaderId;

    if (hasGeometryShader) {
        SL_DEBUG("compiling geometry shader");
        geometryShaderId = compileShader(shader.m_geomPath, GL_GEOMETRY_SHADER);

        glAttachShader(shaderProgramId, geometryShaderId.value());
    }

    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);

    SL_DEBUG("linking shader");
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
        SL_ERROR("could not link: ", infoBuffer);
        throw core::ShaderError { core::ErrorCode::CouldNotLinkShaderProgram };
    }
}

unsigned int OpenGlShaderCompilerImpl::compileShader(const std::string& path, unsigned type) {
    char infoBuffer[infoBufferSize];
    int compiled = 0;

    std::fstream shaderSource(path);

    if (!shaderSource.good()) {
        SL_ERROR("could not find source: {}", path);
        auto code = type == GL_VERTEX_SHADER ? ErrorCode::CouldNotReadVertexShader : ErrorCode::CouldNotReadFragmentShader;
        throw ShaderError { code };
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
        SL_ERROR("could not compile: {}", infoBuffer);
        auto code = type == GL_VERTEX_SHADER ? ErrorCode::CouldNotCompileVertexShader : ErrorCode::CouldNotCompileFragmentShader;
        throw ShaderError { code };
    }

    return shader;
}
}
