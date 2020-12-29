#pragma once

#include "sl/ecs/Component.h"

#include "sl/math/Vector.hpp"
#include "sl/core/misc/colors.hpp"

namespace sl::scene::components {

struct MaterialComponent : public ecs::Component {
    explicit MaterialComponent(math::Vec3 ambientColor = core::misc::COL_WHITE, math::Vec3 diffuseColor = core::misc::COL_WHITE,
        math::Vec3 specularColor = core::misc::COL_WHITE, float shininess = 32.0f)
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