#pragma once

#include "starlight/core/Core.h"
#include "starlight/core/math/Core.h"

namespace sl {

// TODO: move some common stuff to the base class
struct Camera {
    virtual Mat4f getViewMatrix() const       = 0;
    virtual Mat4f getProjectionMatrix() const = 0;
    virtual Vec3f getPosition() const         = 0;

    virtual void onViewportResize(Vec2u32) = 0;

    virtual void update(float deltaTime) = 0;
};

}  // namespace sl