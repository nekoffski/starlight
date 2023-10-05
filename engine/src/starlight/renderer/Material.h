#pragma once

#include <optional>
#include <string>

#include "starlight/core/math/Glm.h"

#include "Texture.h"
#include "TextureMap.h"

#include "starlight/core/utils/Id.h"

namespace sl {

struct Material {
    enum class Type : unsigned char { world, ui };

    uint32_t generation;
    uint32_t internalId;
    std::string name;
    Vec4f diffuseColor;
    TextureMap diffuseMap;

    Id id = idGenerator++;

    Type type;

    inline static Id idGenerator = 0;
};

inline std::optional<Material::Type> materialTypeFromString(
  const std::string& material
) {
    if (material == "ui")
        return Material::Type::ui;
    else if (material == "world")
        return Material::Type::world;

    return {};
}

}  // namespace sl
