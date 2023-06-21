#pragma once

#include "starlight/core/math/Glm.h"

namespace sl {

struct Camera {
    virtual Mat4f getViewMatrix() const       = 0;
    virtual Mat4f getProjectionMatrix() const = 0;
    virtual Vec3f getPosition() const         = 0;

    virtual void update(float deltaTime) {}
};

}  // namespace sl