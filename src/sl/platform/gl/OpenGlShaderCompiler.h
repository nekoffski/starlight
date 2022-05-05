#pragma once

#include "sl/gpu/Shader.h"
#include "sl/gpu/ShaderCompiler.h"

namespace sl::platform::gl {

class OpenGlShader;

class OpenGlShaderCompiler : public gpu::ShaderCompiler {
   public:
    void compile(sl::gpu::Shader& shader) override;

   private:
    void compile(OpenGlShader&);

    unsigned int compileShader(const std::string&, unsigned);

    inline static constexpr int infoBufferSize{1024};
};

}  // namespace sl::platform::gl
