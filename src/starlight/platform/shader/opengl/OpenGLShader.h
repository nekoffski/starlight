#pragma once

#include <starlight/platform/shader/Shader.h>

namespace starl::platform::shader::opengl {

class OpenGLShader : public Shader {
public:
    using Shader::Shader;

    void enable() override;
    void disable() override;

private:
    unsigned int m_shaderProgram{ 0 };
};
}