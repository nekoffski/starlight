#include "Cubemap.h"

#include <ranges>

#include "TextureManager.h"
#include "sl/async/AsyncManager.hpp"
#include "sl/gfx/Texture.h"

namespace sl::gfx {

std::unique_ptr<Cubemap> Cubemap::clone() {
    return TextureManager::get().createCubemap().withWidth(width).withHeight(height).get();
}

}  // namespace sl::gfx