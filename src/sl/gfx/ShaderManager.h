#pragma once

#include <string>

#include "Shader.h"
#include "ShaderCompiler.h"
#include "sl/core/Singleton.hpp"
#include "sl/core/String.hpp"

namespace sl::gfx {

class ShaderManager : public core::Singleton<ShaderManager> {
public:
    std::shared_ptr<Shader> load(const std::string& vertex, const std::string& fragment, const std::string& geometry = "") {
        auto shader = m_shaderFactory->create(vertex, fragment, geometry);
        m_shaderCompiler->compile(*shader);

        shader->name = core::extractNameFromPath(vertex);

        return shader;
    }

    void recompileShader(Shader& shader) {
        m_shaderCompiler->compile(shader);
    }

    void setShaderCompiler(ShaderCompiler* shaderCompiler) {
        m_shaderCompiler = std::move(shaderCompiler);
    }

    void setShaderFactory(Shader::Factory* shaderFactory) {
        m_shaderFactory = shaderFactory;
    }

private:
    ShaderCompiler* m_shaderCompiler;
    Shader::Factory* m_shaderFactory;
};

}