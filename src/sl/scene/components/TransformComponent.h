#pragma once

#include "sl/ecs/Component.h"

#include <memory>

#include "sl/gui/Utils.hpp"
#include "sl/gui/Window.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct TransformComponent : public ecs::Component {
    TransformComponent(math::Vec3 position = math::Vec3{ 0.0f },
        math::Vec3 rotation = math::Vec3{ 0.0f }, math::Vec3 scale = math::Vec3{ 1.0f })
        : position(position)
        , rotation(rotation)
        , scale(scale) {
    }

    math::Vec3 position;
    math::Vec3 rotation;
    math::Vec3 scale;

    math::Mat4 operator()() {
        return math::scale(scale) * math::translate(position) * math::createRotationMatrix(math::toRadians(rotation));
    }
};
}
