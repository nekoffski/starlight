#pragma once

#include "Texture.h"

namespace nova::gfx {

struct TextureMap {
    Texture* texture;
    Texture::Use use;
};

}  // namespace nova::gfx
