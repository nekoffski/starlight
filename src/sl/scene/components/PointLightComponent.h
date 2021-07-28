#pragma once

#include "sl/core/Colors.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gfx/TextureManager.h"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct PointLightComponent : ecs::Component {
    using View = ecs::ComponentView<PointLightComponent>;

    explicit PointLightComponent(math::Vec3 position = math::Vec3 { 0.0f, 0.0f, 0.0f }, math::Vec3 color = core::color::white,
        float attenuationA = 0.05f, float attenuationB = 0.1f, float attenuationC = 0.3f)
        : position(position)
        , color(color)
        , attenuationA(attenuationA)
        , attenuationB(attenuationB)
        , attenuationC(attenuationC)
        , omnidirectionalShadowMap(gfx::TextureManager::get()->createOmnidirectionalShaderMap()) {

        name = "PointLightComponent";
    }

    math::Vec3 position;
    math::Vec3 color;
    float attenuationA;
    float attenuationB;
    float attenuationC;

    std::unique_ptr<gfx::Cubemap> omnidirectionalShadowMap;
};
}
