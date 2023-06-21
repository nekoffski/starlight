#pragma once

#include "Texture.h"

namespace sl {

struct TextureMap {
    Texture* texture;
    Texture::Use use;
};

}  // namespace sl
