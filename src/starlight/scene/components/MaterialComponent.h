#pragma once

#include "starlight/ecs/Component.h"

#include "starlight/math/Vector.hpp"
#include "starlight/misc/colors.hpp"

namespace sl::scene::components {

struct MaterialComponent : public ecs::Component {
    explicit MaterialComponent(math::Vec3 ambientColor = misc::COL_WHITE, math::Vec3 diffuseColor = misc::COL_WHITE,
        math::Vec3 specularColor = misc::COL_WHITE, float shininess = 32.0f)
        : ambientColor(ambientColor)
        , diffuseColor(diffuseColor)
        , specularColor(specularColor)
        , shininess(shininess) {
    }

    math::Vec3 ambientColor;
    math::Vec3 diffuseColor;
    math::Vec3 specularColor;

    float shininess;
};
}