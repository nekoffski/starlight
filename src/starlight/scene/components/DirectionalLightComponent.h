#pragma once

#include "starlight/ecs/Component.h"

#include "starlight/math/Vector.hpp"

namespace sl::scene::components {

struct DirectionalLightComponent : public ecs::Component {
    explicit DirectionalLightComponent(math::Vec3 position, math::Vec3 ambientColor, math::Vec3 diffuseColor)
        : position(position)
        , ambientColor(ambientColor)
        , diffuseColor(diffuseColor) {
    }

    math::Vec3 position;
    math::Vec3 ambientColor;
    math::Vec3 diffuseColor;
};
}