#pragma once

#include <memory>

#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gui/Utils.hpp"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"

namespace sl::scene::components {

struct TransformComponent : ecs::Component {
    explicit TransformComponent(math::Vec3 position = math::Vec3 { 0.0f },
        math::Vec3 rotation = math::Vec3 { 0.0f }, math::Vec3 scale = math::Vec3 { 1.0f })
        : position(position)
        , rotation(rotation)
        , scale(scale) {
        recalculateTransformation();
    }

    void recalculateTransformation() {
        transformation = math::scale(scale) * math::translate(position) * math::createRotationMatrix(math::toRadians(rotation));
    }

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
        entity->addComponent<TransformComponent>(
            deserializeVector3(componentDescription["position"]),
            deserializeVector3(componentDescription["rotation"]),
            deserializeVector3(componentDescription["scale"]));
    }

    math::Vec3 position;
    math::Vec3 rotation;
    math::Vec3 scale;
    math::Mat4 transformation;
};
}
