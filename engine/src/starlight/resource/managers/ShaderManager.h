#pragma once

#include "starlight/renderer/gpu/Shader.h"
#include "starlight/renderer/gpu/ResourcePools.h"

#include "ResourceManager.hpp"
#include "TextureManager.h"

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
