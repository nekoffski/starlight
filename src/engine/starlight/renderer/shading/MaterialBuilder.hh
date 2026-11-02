#pragma once

#include <type_traits>
#include <utility>

#include "Material.hh"

namespace sl {

class MaterialBuilder {
   public:
    explicit MaterialBuilder(ShaderInterface shaderInterface)
        : m_shaderInterface(std::move(shaderInterface)) {}

    MaterialBuilder& addParameter(ShaderParameterBlock block) {
        m_parameters.blocks.push_back(std::move(block));
        return *this;
    }

    template <typename T>
        requires std::is_trivially_copyable_v<T>
    MaterialBuilder& addParameter(Str name, const T& value) {
        return addParameter(
            ShaderParameterBlock::fromValue(std::move(name), value)
        );
    }

    MaterialBuilder& addTextureDependency(TextureHandle texture) {
        m_parameters.resources.textures.push_back(texture);
        return *this;
    }

    MaterialBuilder& addSamplerDependency(SamplerHandle sampler) {
        m_parameters.resources.samplers.push_back(sampler);
        return *this;
    }

    Result<Material> build() const {
        return Material::create(m_shaderInterface, m_parameters);
    }

   private:
    ShaderInterface m_shaderInterface;
    ShaderParameters m_parameters;
};

}  // namespace sl
