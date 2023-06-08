#pragma once

#include <string>

#include "starlight/math/Glm.h"

#include "Texture.h"
#include "TextureMap.h"

#include "starlight/core/Id.h"

namespace sl::gfx {

struct Material {
    uint32_t generation;
    uint32_t internalId;
    std::string name;
    math::Vec4f diffuseColor;
    TextureMap diffuseMap;

    core::Id id = idGenerator++;

    inline static core::Id idGenerator = 0;
};

}  // namespace sl::gfx