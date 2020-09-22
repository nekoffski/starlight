#pragma once

#include "sl/ecs/Component.h"

#include "sl/math/Vector.hpp"
#include "sl/misc/colors.hpp"

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