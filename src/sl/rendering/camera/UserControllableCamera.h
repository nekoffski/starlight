#pragma once

#include "Camera.h"

#include "sl/platform/input/InputHandler.h"

namespace sl::rendering::camera {

class UserControllableCamera : public Camera, public platform::input::InputHandler {
};
}