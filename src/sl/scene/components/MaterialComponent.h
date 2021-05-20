#pragma once

#include "sl/core/Colors.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct MaterialComponent : ecs::Component {
    explicit MaterialComponent(math::Vec3 ambientColor = core::color::white, math::Vec3 diffuseColor = core::color::white,
        math::Vec3 specularColor = core::color::white, float shininess = 32.0f)
        : ambientColor(ambientColor)
        , diffuseColor(diffuseColor)
        , specularColor(specularColor)
        , shininess(shininess) {

        name = "MaterialComponent";
    }

    math::Vec3 ambientColor;
    math::Vec3 diffuseColor;
    math::Vec3 specularColor;

    float shininess;
};
}
