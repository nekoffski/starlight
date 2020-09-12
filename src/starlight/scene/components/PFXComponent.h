#pragma once

#include "starlight/ecs/Component.h"

#include <memory>

namespace sl::scene::components {

struct PFXComponent : public ecs::Component {
    explicit PFXComponent(math::Vec3 position = math::Vec3{ 0.0f, 0.0f, 0.0f })
        : position(position) {
    }

    math::Vec3 position;
};
}
