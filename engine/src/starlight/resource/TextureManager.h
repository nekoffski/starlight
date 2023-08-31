#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/memory/Memory.hpp"
#include "starlight/renderer/fwd.h"

namespace sl {

class TextureManager : public kc::core::Singleton<TextureManager> {
   public:
    explicit TextureManager(TextureLoader& textureLoader, std::string_view texturesPath);

    Texture* load(const std::string& name);
    Texture* acquire(const std::string& name) const;
    Texture* getDefaultTexture() const;

    void destroy(const std::string& name);
    void destroyAll();

   private:
    void loadDefaultTexture();

    TextureLoader& m_textureLoader;
    std::string_view m_texturesPath;

    std::unordered_map<std::string, UniqPtr<Texture>> m_textures;
    UniqPtr<Texture> m_defaultTexture;
};

}  // namespace sl