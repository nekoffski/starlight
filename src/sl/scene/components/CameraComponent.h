#pragma once

#include "sl/cam/FirstPersonCamera.h"
#include "sl/ecs/Component.h"

namespace sl::scene::components {

struct CameraComponent : ecs::Component {
    cam::FirstPersonCamera camera;
};

}  // namespace sl::scene::components