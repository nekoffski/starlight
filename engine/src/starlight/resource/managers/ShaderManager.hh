#pragma once

#include "starlight/renderer/gpu/Shader.hh"
#include "starlight/renderer/gpu/ResourcePools.hh"

#include "ResourceManager.hh"
#include "TextureManager.hh"

namespace sl {

class ShaderManager : public ResourceManager<Shader, ShaderManager> {
public:
    explicit ShaderManager(
      ResourcePools& resourcePools, TextureManager& textureManager
    );
    ~ShaderManager();

    Shader* load(const std::string& name);

private:
    void destroyInternals(Shader* resource) override;

    ResourcePools& m_resourcePools;
    TextureManager& m_textureManager;
};

}  // namespace sl
