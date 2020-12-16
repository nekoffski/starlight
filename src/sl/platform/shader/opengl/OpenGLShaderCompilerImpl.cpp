#include "OpenGLShaderCompilerImpl.h"

#include <cstring>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include "OpenGLShader.h"
#include "sl/core/Logger.h"
#include "sl/platform/Error.h"

#define INFO_BUFFER 1024

namespace sl::platform::shader::opengl {

void OpenGLShaderCompilerImpl::compile(std::shared_ptr<Shader>& shader) {
    compileImpl(std::dynamic_pointer_cast<OpenGLShader>(shader));
}

void OpenGLShaderCompilerImpl::compileImpl(std::shared_ptr<OpenGLShader> shader) {
    SL_DEBUG("compiling shader: \n{},\n{},\n{}", shader->m_fragmentPath, shader->m_vertexPath, shader->m_geometryPath);

    GLuint& _shader_program = shader->m_shaderProgram;
    _shader_program = glCreateProgram();

    SL_DEBUG("compiling vertex shader");
    auto vertexShader = compileShader(shader->m_vertexPath, GL_VERTEX_SHADER);
    SL_DEBUG("compiling fragment shader");
    auto fragmentShader = compileShader(shader->m_fragmentPath, GL_FRAGMENT_SHADER);

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
        throw PlatformException(ErrorCode::COULD_NOT_LINK_SHADER, std::string(info_buffer));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int OpenGLShaderCompilerImpl::compileShader(const std::string& path, unsigned type) {
    char info_buffer[infoBufferSize];
    int compiled = 0;

    std::fstream shaderSource(path);

    if (!shaderSource.good()) {
        SL_ERROR("could not find source: {}", path);
        throw PlatformException(type == GL_VERTEX_SHADER ? ErrorCode::COULD_NOT_OPEN_VERTEX_SHADER : ErrorCode::COULD_NOT_OPEN_FRAGMENT_SHADER,
            "Could not load glsl shader source");
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
        throw PlatformException(type == GL_VERTEX_SHADER ? ErrorCode::COULD_NOT_COMPILE_VERTEX_SHADER : ErrorCode::COULD_NOT_COMPILE_FRAGMENT_SHADER,
            std::string(info_buffer));
    }

    return shader;
}
}