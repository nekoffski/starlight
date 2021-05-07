#pragma once

#include <functional>
#include <unordered_map>

#include "sl/asset/AssetManager.h"
#include "sl/ecs/Entity.h"

namespace sl::app {

class ComponentsDeserializer {
    using DeserializerCallback =
        std::function<void(Json::Value&, ecs::Entity&, asset::AssetManager&)>;

public:
    explicit ComponentsDeserializer();

    void deserializeComponent(const std::string& name, Json::Value& componentDescription,
        ecs::Entity& entity, asset::AssetManager& assetManager);

private:
    static void deserializeDirectionalLightComponent(Json::Value& componentDescription,
        ecs::Entity& entity, asset::AssetManager& assetManager);

    static void deserializeMaterialComponent(Json::Value& componentDescription,
        ecs::Entity& entity, asset::AssetManager& assetManager);

    static void deserializeMeshRendererComponent(Json::Value& componentDescription,
        ecs::Entity& entity, asset::AssetManager& assetManager);

    static void deserializeModelComponent(Json::Value& componentDescription,
        ecs::Entity& entity, asset::AssetManager& assetManager);

    static void deserializeParticleEffectComponent(Json::Value& componentDescription,
        ecs::Entity& entity, asset::AssetManager& assetManager);

    static void deserializePointLightComponent(Json::Value& componentDescription,
        ecs::Entity& entity, asset::AssetManager& assetManager);

    static void deserializeTransformComponent(Json::Value& componentDescription,
        ecs::Entity& entity, asset::AssetManager& assetManager);

    static void deserializeRigidBodyComponent(Json::Value& componentDescription,
        ecs::Entity& entity, asset::AssetManager& assetManager);

    static math::Vec3 deserializeVector3(Json::Value& value);

    std::unordered_map<std::string, DeserializerCallback> m_deserializers;
};
}