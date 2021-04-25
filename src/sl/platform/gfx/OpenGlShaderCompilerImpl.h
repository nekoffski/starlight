#pragma once

#include "sl/gfx/Shader.h"
#include "sl/gfx/ShaderCompilerImpl.h"

namespace sl::platform::gfx {

class OpenGlShader;

class OpenGlShaderCompilerImpl : public sl::gfx::ShaderCompilerImpl {
public:
    void compile(sl::gfx::Shader& shader) override;

private:
    void compileImpl(OpenGlShader&);
    unsigned int compileShader(const std::string&, unsigned);

    inline static constexpr int infoBufferSize { 1024 };
};

struct OpenGlShaderCompilerImplFactory : sl::gfx::ShaderCompilerImpl::Factory {
    std::unique_ptr<sl::gfx::ShaderCompilerImpl> create() override {
        return std::make_unique<OpenGlShaderCompilerImpl>();
    }
};
}
