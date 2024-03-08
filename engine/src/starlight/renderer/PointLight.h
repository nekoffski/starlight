#pragma once

#include "starlight/core/math/Glm.h"

namespace sl {

struct PointLight {
    Vec4f position           = Vec4f{ 0.0f, 0.0f, 0.0, 1.0f };
    Vec4f color              = Vec4f{ 1.0f };
    Vec4f attenuationFactors = Vec4f{ 1.0f, 0.2f, 0.33f, 0.0f };
};

}  // namespace sl
