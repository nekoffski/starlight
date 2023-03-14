#pragma once

#include <string>

#include "nova/math/Glm.h"

#include "Texture.h"
#include "TextureMap.h"

#include "nova/core/Id.h"

namespace nova::gfx {

struct Material {
    uint32_t generation;
    uint32_t internalId;
    std::string name;
    math::Vec4f diffuseColor;
    TextureMap diffuseMap;

    core::Id id = idGenerator++;

    inline static core::Id idGenerator = 0;
};

}  // namespace nova::gfx