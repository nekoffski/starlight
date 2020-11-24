#pragma once

#include "Camera.h"

#include "sl/platform/input/InputHandler.h"

namespace sl::rendering::camera {

class UserControllableCamera : public Camera, public platform::input::InputHandler {
public:
    explicit UserControllableCamera(math::Vec3 position = math::Vec3{ 0.0f }, math::Vec3 up = WORLD_UP,
        math::Vec3 front = WORLD_FRONT, math::Vec3 right = WORLD_RIGHT)
        : Camera(position, up, front, right) {
    }
};
}
