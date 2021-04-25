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

    GLuint& _shader_program = shader.m_shaderProgram;
    _shader_program = glCreateProgram();

    SL_DEBUG("compiling vertex shader");
    auto vertexShader = compileShader(shader.m_vertexPath, GL_VERTEX_SHADER);
    SL_DEBUG("compiling fragment shader");
    auto fragmentShader = compileShader(shader.m_fragmentPath, GL_FRAGMENT_SHADER);

    glAttachShader(_shader_program, vertexShader);
    glAttachShader(_shader_program, fragmentShader);

    SL_DEBUG("linking shader");
    glLinkProgram(_shader_program);

    char info_buffer[infoBufferSize];
    int linked = 0;
    std::memset(info_buffer, 0, infoBufferSize);

    glGetProgramiv(_shader_program, GL_LINK_STATUS, &linked);

    if (!linked) {
        glGetProgramInfoLog(_shader_program, infoBufferSize, nullptr, info_buffer);
        SL_ERROR("could not link: ", info_buffer);
        throw core::ShaderError { core::ErrorCode::CouldNotLinkShaderProgram };
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int OpenGlShaderCompilerImpl::compileShader(const std::string& path, unsigned type) {
    char info_buffer[infoBufferSize];
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

    std::memset(info_buffer, 0, infoBufferSize);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        glGetShaderInfoLog(shader, infoBufferSize, nullptr, info_buffer);
        SL_ERROR("could not compile: {}", info_buffer);
        auto code = type == GL_VERTEX_SHADER ? ErrorCode::CouldNotCompileVertexShader : ErrorCode::CouldNotCompileFragmentShader;
        throw ShaderError { code };
    }

    return shader;
}
}
