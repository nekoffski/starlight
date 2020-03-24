#pragma once

#include <starlight/framework/graphics/camera/Camera.h>
#include <starlight/platform/input/InputHandler.h>

namespace starl::framework::graphics::camera {

class UserControllableCamera : public Camera, public platform::input::InputHandler {
};
}