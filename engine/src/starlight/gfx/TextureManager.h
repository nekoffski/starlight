#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "starlight/core/Memory.hpp"

#include "fwd.h"

namespace sl::gfx {

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

    std::unordered_map<std::string, core::UniqPtr<Texture>> m_textures;
    core::UniqPtr<Texture> m_defaultTexture;
};

}  // namespace sl::gfx