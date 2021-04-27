#pragma once

#include <functional>
#include <unordered_map>

#include "sl/asset/AssetManager.h"
#include "sl/core/Json.h"
#include "sl/ecs/Entity.h"

#include "DirectionalLightComponent.h"
#include "MaterialComponent.h"
#include "MeshRendererComponent.h"
#include "ModelComponent.h"
#include "PointLightComponent.h"
#include "TransformComponent.h"

namespace sl::scene::components {

using DeserializeCallback = std::function<void(std::shared_ptr<ecs::Entity>, asset::AssetManager&, Json::Value&)>;

static std::unordered_map<std::string, DeserializeCallback> componentsDeserializers {
    { "TransformComponent", &TransformComponent::deserialize },
    { "ModelComponent", &ModelComponent::deserialize },
    { "MeshRendererComponent", &MeshRendererComponent::deserialize },
    { "DirectionalLightComponent", &DirectionalLightComponent::deserialize },
    { "MaterialComponent", &MaterialComponent::deserialize },
    { "PointLightComponent", &PointLightComponent::deserialize }
};
}
