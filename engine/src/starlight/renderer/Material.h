#pragma once

#include <string>

#include "starlight/core/math/Glm.h"

#include "Texture.h"
#include "TextureMap.h"

#include "starlight/core/Id.h"

namespace sl {

struct Material {
    uint32_t generation;
    uint32_t internalId;
    std::string name;
    Vec4f diffuseColor;
    TextureMap diffuseMap;

    Id id = idGenerator++;

    inline static Id idGenerator = 0;
};

}  // namespace sl