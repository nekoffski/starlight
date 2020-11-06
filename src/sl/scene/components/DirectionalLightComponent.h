#pragma once

#include "sl/ecs/Component.h"

#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"
#include "sl/misc/colors.hpp"
#include "sl/platform/texture/Texture.h"

namespace sl::scene::components {

static const auto LIGHT_PROJECTION = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);

struct DirectionalLightComponent : public ecs::Component {
    explicit DirectionalLightComponent(math::Vec3 direction, math::Vec3 color = misc::COL_WHITE)
        : direction(direction)
        , color(color)
        , shadowMap(platform::texture::Texture::create(1024u, 1024u))
        , viewMatrix(math::lookAt(direction, math::Vec3{ 0.0f }, math::Vec3{ 0.0f, 1.0f, 0.0f }))
        , spaceMatrix(LIGHT_PROJECTION * viewMatrix) {
    }

    math::Vec3 direction;
    math::Vec3 color;
    math::Mat4 viewMatrix;
    math::Mat4 spaceMatrix;
    std::shared_ptr<platform::texture::Texture> shadowMap;
};
}
