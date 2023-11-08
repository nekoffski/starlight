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

    explicit TextureManager(RendererProxy& rendererProxy);
    ~TextureManager();

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

    RendererProxy& m_rendererProxy;

    std::unordered_map<std::string, Texture*> m_textures;

    Texture* m_defaultTexture;
    Texture* m_defaultNormalMap;
    Texture* m_defaultSpecularMap;
};

}  // namespace sl