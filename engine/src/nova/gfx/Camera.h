#pragma once

#include "nova/math/Glm.h"

namespace nova::gfx {

struct Camera {
    virtual math::Mat4f getViewMatrix()       = 0;
    virtual math::Mat4f getProjectionMatrix() = 0;
};

}  // namespace nova::gfx