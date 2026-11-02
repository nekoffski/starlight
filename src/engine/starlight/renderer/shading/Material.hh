#pragma once

#include "starlight/renderer/core/Shader.hh"
#include "starlight/renderer/core/Texture.hh"

namespace sl {

struct ShaderParameters {
    std::vector<ShaderParameterBlock> blocks;
    ShaderResourceDependencies resources;
};

class Material {
   public:
    static Result<Material> create(
        ShaderInterface shaderInterface, ShaderParameters parameters
    );

    const ShaderInterface& shaderInterface() const { return m_shaderInterface; }

    const ShaderParameters& parameters() const { return m_parameters; }

   private:
    Material(ShaderInterface shaderInterface, ShaderParameters parameters);

    ShaderInterface m_shaderInterface;
    ShaderParameters m_parameters;
};

}  // namespace sl
