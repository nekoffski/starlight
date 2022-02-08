#pragma once

#include "sl/ecs/Component.h"

#include "sl/cam/FirstPersonCamera.h"

namespace sl::scene::components {

struct CameraComponent : ecs::Component {
    cam::FirstPersonCamera camera;
};

}