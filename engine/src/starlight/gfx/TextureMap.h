#pragma once

#include "Texture.h"

namespace sl::gfx {

struct TextureMap {
    Texture* texture;
    Texture::Use use;
};

}  // namespace sl::gfx
