#pragma once

#include <memory>

#include <starlight/platform/shader/ShaderCompilerImpl.h>

namespace starl::platform::shader {

class Shader;

class ShaderCompiler {
public:
    static void init() {
        m_shaderCompilerImpl = ShaderCompilerImpl::create();
    }

    static void compile(std::shared_ptr<Shader>& shader) {
        m_shaderCompilerImpl->compile(shader);
    }

private:
    inline static std::unique_ptr<ShaderCompilerImpl> m_shaderCompilerImpl{ nullptr };
};
}