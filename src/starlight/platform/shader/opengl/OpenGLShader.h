#pragma once

#include <starlight/platform/shader/Shader.h>

namespace starl::platform::shader::opengl {

class OpenGLShaderCompilerImpl;

class OpenGLShader : public Shader {
    friend class OpenGLShaderCompilerImpl;

public:
    explicit OpenGLShader(std::string, std::string, std::string);

    ~OpenGLShader() override;

    void enable() override;
    void disable() override;

private:
    unsigned int m_shaderProgram{ 0 };

    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::string m_geometryPath;
};
}