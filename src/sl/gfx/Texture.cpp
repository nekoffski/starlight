#include "Texture.h"

#include "TextureManager.h"
#include "sl/async/AsyncManager.hpp"

namespace sl::gfx {

std::unique_ptr<Texture> Texture::clone() {
    return TextureManager::get()
        .createTexture()
        .withWidth(getWidth())
        .withHeight(getHeight())
        .withColorComponents(internalFormat)
        .withFormat(format)
        .get();
}

}