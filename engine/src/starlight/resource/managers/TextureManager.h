#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/Core.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/fwd.h"

namespace sl {

class TextureManager {
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
    Texture* acquire(const std::string& name) const;
    Texture* acquire(u64 id) const;

    template <typename Callback>
    requires Callable<Callback, void, std::string, const Texture*>
    void forEach(Callback&& callback) {
        for (const auto [name, texture] : m_textures) callback(name, texture);
    }

    void destroy(const std::string& name);
    void destroyAll();

private:
    void createDefaultTextureMap();
    void createDefaultTexture();
    void createDefaultSpecularMap();
    void createDefaultNormalMap();

    ResourcePools& m_resourcePools;

    std::unordered_map<std::string, Texture*> m_textures;
    std::unordered_map<u64, Texture*> m_texturesById;

    Texture* m_defaultTexture;
    Texture* m_defaultNormalMap;
    Texture* m_defaultSpecularMap;
};

}  // namespace sl