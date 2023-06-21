#pragma once

#include <cstdint>

#include "starlight/core/math/Glm.h"

namespace sl {

struct GlobalState {
    Mat4f projectionMatrix;
    Mat4f viewMatrix;
    Vec3f viewPosition;
    Vec4f ambientColor;

    int32_t mode;
};
}  // namespace sl