#pragma once

#include "sl/ecs/Component.h"

#include "sl/math/Vector.hpp"
#include "sl/misc/colors.hpp"
#include "sl/platform/texture/Texture.h"

namespace sl::scene::components {

struct DirectionalLightComponent : public ecs::Component {
    explicit DirectionalLightComponent(math::Vec3 direction, math::Vec3 color = misc::COL_WHITE)
        : direction(direction)
        , color(color)
        , shadowMap(platform::texture::Texture::create(1024u, 1024u)) {
    }

    math::Vec3 direction;
    math::Vec3 color;
    std::shared_ptr<platform::texture::Texture> shadowMap;
};
}
