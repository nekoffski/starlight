#pragma once

#include "starlight/core/Memory.hpp"

#include "Texture.h"

namespace sl::gfx {

struct TextureLoader {
    virtual core::UniqPtr<Texture> load(
        const std::string& name, const Texture::Properties& props, const void* pixels
    ) const = 0;

    virtual core::UniqPtr<Texture> load(const std::string& name) const = 0;
};

}  // namespace sl::gfx
