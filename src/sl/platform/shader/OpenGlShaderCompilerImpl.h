#pragma once

#include "sl/gfx/Shader.h"
#include "sl/gfx/ShaderCompilerImpl.h"

namespace sl::platform::shader {

class OpenGlShader;

class OpenGlShaderCompilerImpl : public gfx::ShaderCompilerImpl {
public:
    void compile(std::shared_ptr<gfx::Shader>& shader) override;

private:
    void compileImpl(std::shared_ptr<OpenGlShader>);
    unsigned int compileShader(const std::string&, unsigned);

    inline static constexpr int infoBufferSize{ 1024 };
};

struct OpenGlShaderCompilerImplFactory : gfx::ShaderCompilerImpl::Factory {
    std::unique_ptr<gfx::ShaderCompilerImpl> create() override {
        return std::make_unique<OpenGlShaderCompilerImpl>();
    }
};
}
