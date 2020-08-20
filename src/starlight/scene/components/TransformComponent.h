#pragma once

#include "starlight/ecs/Component.h"

#include <memory>

#include "starlight/gui/Utils.hpp"
#include "starlight/gui/Window.h"
#include "starlight/math/Utils.hpp"
#include "starlight/math/Vector.hpp"

namespace sl::scene::components {

struct TransformComponent : public ecs::Component {
    math::Vec3 translation{ 0.0f };
    math::Vec3 rotation{ 0.0f };

    math::Mat4 operator()() {
        return math::translate(translation) *
            math::createRotationMatrix(math::toRadians(rotation));
    }
};
}