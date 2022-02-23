#pragma once

#include "sl/gfx/Shader.h"
#include "sl/gfx/ShaderCompiler.h"

namespace sl::platform::gl {

class OpenGlShader;

class OpenGlShaderCompiler : public gfx::ShaderCompiler {
   public:
    void compile(sl::gfx::Shader& shader) override;

   private:
    void compile(OpenGlShader&);

    unsigned int compileShader(const std::string&, unsigned);

    inline static constexpr int infoBufferSize{1024};
};

}  // namespace sl::platform::gl
