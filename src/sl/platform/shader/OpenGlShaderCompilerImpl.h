#pragma once

#include "sl/graphics/Shader.h"
#include "sl/graphics/ShaderCompilerImpl.h"

namespace sl::platform::shader {

class OpenGlShader;

class OpenGlShaderCompilerImpl : public graphics::ShaderCompilerImpl {
public:
    void compile(std::shared_ptr<graphics::Shader>& shader) override;

private:
    void compileImpl(std::shared_ptr<OpenGlShader>);
    unsigned int compileShader(const std::string&, unsigned);

    inline static constexpr int infoBufferSize{ 1024 };
};

struct OpenGlShaderCompilerImplFactory : graphics::ShaderCompilerImpl::Factory {
    std::unique_ptr<graphics::ShaderCompilerImpl> create() override {
        return std::make_unique<OpenGlShaderCompilerImpl>();
    }
};
}
