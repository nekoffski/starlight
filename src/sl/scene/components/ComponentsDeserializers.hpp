#pragma once

#include <functional>
#include <unordered_map>

#include "sl/asset/AssetManager.h"
#include "sl/core/Json.h"
#include "sl/ecs/Entity.h"

#include "ModelComponent.h"
#include "RendererComponent.h"
#include "TransformComponent.h"

namespace sl::scene::components {

using DeserializeCallback = std::function<void(std::shared_ptr<ecs::Entity>, asset::AssetManager&, Json::Value&)>;

static std::unordered_map<std::string, DeserializeCallback> componentsDeserializers{
    { "TransformComponent", &TransformComponent::deserialize },
    { "ModelComponent", &ModelComponent::deserialize },
    { "RendererComponent", &RendererComponent::deserialize }
};
}
