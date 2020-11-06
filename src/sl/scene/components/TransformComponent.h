#pragma once

#include "sl/ecs/Component.h"

#include <memory>

#include "sl/gui/Utils.hpp"
#include "sl/gui/Window.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct TransformComponent : public ecs::Component {
    math::Vec3 position{ 0.0f };
    math::Vec3 rotation{ 0.0f };
    math::Vec3 scale{ 1.0f };

    math::Mat4 operator()() {
        return math::scale(scale) * math::translate(position) *
            math::createRotationMatrix(math::toRadians(rotation));
    }
};
}
