#pragma once

#include <optional>

#include "starlight/core/math/Glm.h"
#include "starlight/renderer/Material.h"

namespace sl {

struct MaterialConfig {
    Vec4f diffuseColor;
    std::string diffuseMap;
    Material::Type type;
};

}  // namespace sl
