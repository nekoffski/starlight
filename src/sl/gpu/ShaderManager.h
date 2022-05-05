#pragma once

#include <kc/core/String.h>

#include <kc/core/Singleton.hpp>
#include <string>

#include "Shader.h"
#include "ShaderCompiler.h"

namespace sl::gpu {

class ShaderManager : public kc::core::Singleton<ShaderManager> {
   public:
    explicit ShaderManager(ShaderCompiler* shaderCompiler, Shader::Factory* shaderFactory);

    std::shared_ptr<Shader> load(
        const std::string& vertex, const std::string& fragment, const std::string& geometry = ""
    );

    void recompileShader(Shader& shader);

   private:
    ShaderCompiler* m_shaderCompiler;
    Shader::Factory* m_shaderFactory;
};

}  // namespace sl::gpu