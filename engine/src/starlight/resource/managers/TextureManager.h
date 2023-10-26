#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/fwd.h"

#include "starlight/resource/ResourceLoader.h"

namespace sl {

class TextureManager {
public:
    inline static const std::string defaultNormalMapName =
      "Internal.Texture.DefaultNormalMap";
    inline static const std::string defaultTextureName = "Internal.Texture.Default";
    inline static const std::string defaultSpecularMapName =
      "Internal.Texture.DefaultSpecularMap";

    explicit TextureManager(
      TextureLoader& textureLoader, const ResourceLoader& resourceLoader
    );

    Texture* load(const std::string& name);
    Texture* acquire(const std::string& name) const;

    Texture* getDefaultTexture() const;
    Texture* getDefaultNormalMap() const;
    Texture* getDefaultSpecularMap() const;

    void destroy(const std::string& name);
    void destroyAll();

private:
    void createDefaultTexture();
    void createDefaultSpecularMap();
    void createDefaultNormalMap();

    TextureLoader& m_textureLoader;
    const ResourceLoader& m_resourceLoader;

    std::unordered_map<std::string, UniqPtr<Texture>> m_textures;

    UniqPtr<Texture> m_defaultTexture;
    UniqPtr<Texture> m_defaultNormalMap;
    UniqPtr<Texture> m_defaultSpecularMap;
};

}  // namespace sl