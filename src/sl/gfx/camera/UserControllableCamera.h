#pragma once

#include "Camera.h"

#include "sl/core/InputHandler.h"
#include "sl/gfx/ViewFrustum.h"

namespace sl::gfx::camera {

class UserControllableCamera : public Camera, public core::InputHandler {
public:
    explicit UserControllableCamera(const ViewFrustum& viewFrustum, math::Vec3 position = math::Vec3{ 0.0f }, math::Vec3 up = worldUp,
        math::Vec3 front = worldFront, math::Vec3 right = worldRight)
        : Camera(viewFrustum, position, up, front, right) {
    }
};
}
