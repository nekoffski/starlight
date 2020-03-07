#pragma once

#include <starlight/platform/shader/ShaderCompilerImpl.h>

namespace starl::platform::shader {
class Shader;
}

namespace starl::platform::shader::opengl {

class OpenGLShaderCompilerImpl : public ShaderCompilerImpl {
public:
    void compile(std::shared_ptr<Shader>&);
};
}