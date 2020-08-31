#pragma once

#include "starlight/ecs/Component.h"

#include "starlight/math/Vector.hpp"
#include "starlight/misc/colors.hpp"

namespace sl::scene::components {

struct PointLightComponent : public ecs::Component {
    explicit PointLightComponent(math::Vec3 position = math::Vec3{ 0.0f, 0.0f, 0.0f }, math::Vec3 color = misc::COL_WHITE,
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