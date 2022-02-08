#pragma once

#include <string>

#include <kc/core/Singleton.hpp>
#include <kc/core/String.h>

#include "Shader.h"
#include "ShaderCompiler.h"

namespace sl::gfx {

class ShaderManager : public kc::core::Singleton<ShaderManager> {
public:
    explicit ShaderManager(ShaderCompiler* shaderCompiler, Shader::Factory* shaderFactory);

    std::shared_ptr<Shader> load(const std::string& vertex, const std::string& fragment, const std::string& geometry = "");

    void recompileShader(Shader& shader);

private:
    ShaderCompiler* m_shaderCompiler;
    Shader::Factory* m_shaderFactory;
};

}