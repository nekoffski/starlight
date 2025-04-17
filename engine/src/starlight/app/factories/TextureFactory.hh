#pragma once

#include "starlight/core/Factory.hh"
#include "starlight/renderer/gpu/Texture.hh"
#include "starlight/core/Json.hh"

namespace sl {

class TextureFactory : public Factory<TextureFactory, Texture, Texture::Type> {
public:
    explicit TextureFactory();

    kstd::SharedPtr<Texture> load(
      const std::string& name, Texture::Type textureType,
      const Texture::SamplerProperties& sampler =
        Texture::SamplerProperties::createDefault()
    );

    kstd::SharedPtr<Texture> getDefaultDiffuseMap();
    kstd::SharedPtr<Texture> getDefaultNormalMap();
    kstd::SharedPtr<Texture> getDefaultSpecularMap();

private:
    void createDefaults();

    kstd::SharedPtr<Texture> m_defaultDiffuseMap;
    kstd::SharedPtr<Texture> m_defaultNormalMap;
    kstd::SharedPtr<Texture> m_defaultSpecularMap;
};

void serialize(nlohmann::json& j, const kstd::SharedPtr<Texture>& v);
void deserialize(const nlohmann::json& j, kstd::SharedPtr<Texture>& v);

}  // namespace sl
