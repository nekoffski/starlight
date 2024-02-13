#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/Core.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/fwd.h"

#include "ResourceManager.hpp"

namespace sl {

class TextureManager : public ResourceManager<Texture, TextureManager> {
public:
    inline static const std::string defaultNormalMapName =
      "Internal.Texture.DefaultNormalMap";
    inline static const std::string defaultTextureName = "Internal.Texture.Default";
    inline static const std::string defaultSpecularMapName =
      "Internal.Texture.DefaultSpecularMap";

    explicit TextureManager(ResourcePools& resourcePools);
    ~TextureManager();

    Texture* load(const std::string& name);
    Texture* loadCubeTexture(const std::string& name);

private:
    void destroyInternals(Texture* resource) override;

    void createDefaultTextureMap();
    void createDefaultTexture();
    void createDefaultSpecularMap();
    void createDefaultNormalMap();

    ResourcePools& m_resourcePools;

    Texture* m_defaultTexture;
    Texture* m_defaultNormalMap;
    Texture* m_defaultSpecularMap;
};

}  // namespace sl