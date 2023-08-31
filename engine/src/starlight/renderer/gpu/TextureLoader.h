#pragma once

#include "starlight/core/memory/Memory.hpp"

#include "starlight/renderer/Texture.h"

namespace sl {

struct TextureLoader {
    virtual UniqPtr<Texture> load(
        const std::string& name, const Texture::Properties& props, const void* pixels
    ) const = 0;
};

}  // namespace sl
