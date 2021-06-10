#pragma once

#include "sl/core/Colors.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Texture.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

static const auto lightProjectionMatrix = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, 0.01f, 100.0f);

struct DirectionalLightComponent : ecs::Component {
    explicit DirectionalLightComponent(math::Vec3 direction = math::Vec3 { 1.0f, 1.0f, 1.0f }, math::Vec3 color = core::color::white)
        : direction(direction)
        , color(color)
        , shadowMap(gfx::Texture::createShadowMap())
        , renderDirection(false) {

        recalculateMatrices();
        name = "DirectionalLightComponent";
    }

    void recalculateMatrices() {
        viewMatrix = math::lookAt(direction, math::Vec3 { 0.0f }, math::Vec3 { 0.0f, 1.0f, 0.0f });
        spaceMatrix = lightProjectionMatrix * viewMatrix;
    }

    math::Vec3 direction;
    math::Vec3 color;
    math::Mat4 viewMatrix;
    math::Mat4 spaceMatrix;

    std::unique_ptr<sl::gfx::Texture> shadowMap;

    bool renderDirection;
};
}
