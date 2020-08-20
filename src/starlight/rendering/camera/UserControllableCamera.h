#pragma once

#include "Camera.h"

#include "starlight/platform/input/InputHandler.h"

namespace sl::rendering::camera {

class UserControllableCamera : public Camera, public platform::input::InputHandler {
};
}