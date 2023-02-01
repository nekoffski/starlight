#pragma once

#include <kc/core/Singleton.hpp>

#include "Texture.h"

namespace nova::gfx {

class TextureManager : public kc::core::Singleton<TextureManager> {
   public:
    explicit TextureManager(TextureLoader* textureLoader) : m_textureLoader(textureLoader) {}

   private:
    TextureLoader* m_textureLoader;
};

}  // namespace nova::gfx