#pragma once

#include "Camera.h"

#include "starlight/platform/input/InputHandler.h"

namespace sl::framework::graphics::camera {

class UserControllableCamera : public Camera, public platform::input::InputHandler {
};
}