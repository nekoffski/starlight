#pragma once

#include <string>

#include "nova/math/Glm.h"

#include "Texture.h"
#include "TextureMap.h"

namespace nova::gfx {

struct Material {
    uint32_t generation;
    uint32_t internalId;
    std::string name;
    math::Vec4f diffuseColor;
    TextureMap diffuseMap;

    uint32_t id = idGenerator++;

    inline static uint32_t idGenerator = 0;
};

}  // namespace nova::gfx