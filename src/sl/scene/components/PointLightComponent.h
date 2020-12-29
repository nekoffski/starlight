#pragma once

#include "sl/ecs/Component.h"

#include "sl/math/Vector.hpp"
#include "sl/core/misc/colors.hpp"

namespace sl::scene::components {

struct PointLightComponent : public ecs::Component {
    explicit PointLightComponent(math::Vec3 position = math::Vec3{ 0.0f, 0.0f, 0.0f }, math::Vec3 color = core::misc::COL_WHITE,
        float attenuationA = 0.05f, float attenuationB = 0.1f, float attenuationC = 0.3f)
        : position(position)
        , color(color)
        , attenuationA(attenuationA)
        , attenuationB(attenuationB)
        , attenuationC(attenuationC) {
    }

    math::Vec3 position;
    math::Vec3 color;
    float attenuationA;
    float attenuationB;
    float attenuationC;
};
}