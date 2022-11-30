#pragma once

#include "nova/math/Glm.h"

namespace nova::gfx {

struct Camera {
    virtual math::Mat4f getViewMatrix() const       = 0;
    virtual math::Mat4f getProjectionMatrix() const = 0;
    virtual math::Vec3f getPosition() const         = 0;

    virtual void update(float deltaTime) {}
};

}  // namespace nova::gfx