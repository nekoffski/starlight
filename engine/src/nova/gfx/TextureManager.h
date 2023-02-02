#pragma once

#include <unordered_map>

#include <kc/core/Singleton.hpp>

#include "nova/core/Memory.hpp"

#include "fwd.h"

namespace nova::gfx {

class TextureManager : public kc::core::Singleton<TextureManager> {
   public:
    explicit TextureManager(TextureLoader* textureLoader, std::string_view texturesPath);

    Texture* load(const std::string& name, const std::string& path);
    Texture* acquire(const std::string& name);

    void destroy(const std::string& name);
    void destroyAll();

   private:
    TextureLoader* m_textureLoader;
    std::string_view m_texturesPath;

    std::unordered_map<std::string, core::UniqPtr<Texture>> m_textures;
};

}  // namespace nova::gfx