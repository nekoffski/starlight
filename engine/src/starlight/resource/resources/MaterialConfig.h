#pragma once

#include <optional>

#include "starlight/core/math/Glm.h"
#include "starlight/renderer/Material.h"

namespace sl {

struct MaterialConfig {
    Vec4f diffuseColor;
    float shininess;
    std::string diffuseMap;
    std::string specularMap;
    std::string normalMap;
    std::string shaderName;
};

}  // namespace sl
