#pragma once

#include "starlight/ecs/Component.h"

#include "starlight/math/Vector.hpp"
#include "starlight/misc/colors.hpp"

namespace sl::scene::components {

struct DirectionalLightComponent : public ecs::Component {
    explicit DirectionalLightComponent(math::Vec3 direction, math::Vec3 color = misc::COL_WHITE)
        : direction(direction)
        , color(color) {
    }

    math::Vec3 direction;
    math::Vec3 color;
};
}