#pragma once

#include "nova/core/Memory.hpp"

#include "Texture.h"

namespace nova::gfx {

struct TextureLoader {
    virtual core::UniqPtr<Texture> load(
        const std::string& name, const Texture::Properties& props, const void* pixels
    ) const = 0;

    virtual core::UniqPtr<Texture> load(const std::string& name) const = 0;
};

}  // namespace nova::gfx
