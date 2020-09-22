#pragma once

#include "sl/platform/shader/Shader.h"

namespace sl::platform::shader::opengl {

class OpenGLShaderCompilerImpl;

class OpenGLShader : public Shader {
    friend class OpenGLShaderCompilerImpl;

public:
    explicit OpenGLShader(std::string, std::string, std::string);

    ~OpenGLShader() override;

    void enable() override;
    void disable() override;

    void setUniform(const std::string&, float) override;
    void setUniform(const std::string&, int) override;
    void setUniform(const std::string&, unsigned int) override;
    void setUniform(const std::string&, math::Mat4) override;
    void setUniform(const std::string&, math::Mat3) override;
    void setUniform(const std::string&, math::Vec4) override;
    void setUniform(const std::string&, math::Vec3) override;

private:
    unsigned int m_shaderProgram{ 0 };

    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::string m_geometryPath;
};
}