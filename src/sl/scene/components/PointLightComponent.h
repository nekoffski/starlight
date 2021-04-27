#pragma once

#include "sl/core/Colors.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct PointLightComponent : ecs::Component {
    explicit PointLightComponent(math::Vec3 position = math::Vec3 { 0.0f, 0.0f, 0.0f }, math::Vec3 color = core::colorWhite,
        float attenuationA = 0.05f, float attenuationB = 0.1f, float attenuationC = 0.3f)
        : position(position)
        , color(color)
        , attenuationA(attenuationA)
        , attenuationB(attenuationB)
        , attenuationC(attenuationC) {
    }

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
        auto& attenuation = componentDescription["attenuation"];
        entity->addComponent<PointLightComponent>(
            deserializeVector3(componentDescription["position"]),
            deserializeVector3(componentDescription["color"]),
            attenuation["a"].asFloat(),
            attenuation["b"].asFloat(),
            attenuation["c"].asFloat());
    }

    math::Vec3 position;
    math::Vec3 color;
    float attenuationA;
    float attenuationB;
    float attenuationC;
};
}
