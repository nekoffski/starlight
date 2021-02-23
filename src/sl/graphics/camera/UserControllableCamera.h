#pragma once

#include "Camera.h"

#include "sl/core/InputHandler.h"

namespace sl::graphics::camera {

class UserControllableCamera : public Camera, public core::InputHandler {
public:
    explicit UserControllableCamera(math::Vec3 position = math::Vec3{ 0.0f }, math::Vec3 up = worldUp,
        math::Vec3 front = worldFront, math::Vec3 right = worldRight)
        : Camera(position, up, front, right) {
    }
};
}
