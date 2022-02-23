#pragma once

#include <kc/json/Json.h>

#include <functional>
#include <unordered_map>

#include "sl/asset/AssetManager.h"
#include "sl/ecs/Entity.h"

namespace sl::app {

class ComponentsDeserializer {
    using DeserializerCallback =
        std::function<void(kc::json::Node&, ecs::Entity&, asset::AssetManager&,
                           const std::unordered_map<std::string, std::string>&)>;

   public:
    explicit ComponentsDeserializer(
        const std::unordered_map<std::string, std::string>& assetsIdRedirections);

    void deserializeComponent(const std::string& name, kc::json::Node& componentDescription,
                              ecs::Entity& entity, asset::AssetManager& assetManager);

   private:
    static void deserializeDirectionalLightComponent(
        kc::json::Node& componentDescription, ecs::Entity& entity,
        asset::AssetManager& assetManager,
        const std::unordered_map<std::string, std::string>& assetsIdRedirections);

    static void deserializeMaterialComponent(
        kc::json::Node& componentDescription, ecs::Entity& entity,
        asset::AssetManager& assetManager,
        const std::unordered_map<std::string, std::string>& assetsIdRedirections);

    static void deserializeMeshRendererComponent(
        kc::json::Node& componentDescription, ecs::Entity& entity,
        asset::AssetManager& assetManager,
        const std::unordered_map<std::string, std::string>& assetsIdRedirections);

    static void deserializeModelComponent(
        kc::json::Node& componentDescription, ecs::Entity& entity,
        asset::AssetManager& assetManager,
        const std::unordered_map<std::string, std::string>& assetsIdRedirections);

    static void deserializeParticleEffectComponent(
        kc::json::Node& componentDescription, ecs::Entity& entity,
        asset::AssetManager& assetManager,
        const std::unordered_map<std::string, std::string>& assetsIdRedirections);

    static void deserializePointLightComponent(
        kc::json::Node& componentDescription, ecs::Entity& entity,
        asset::AssetManager& assetManager,
        const std::unordered_map<std::string, std::string>& assetsIdRedirections);

    static void deserializeTransformComponent(
        kc::json::Node& componentDescription, ecs::Entity& entity,
        asset::AssetManager& assetManager,
        const std::unordered_map<std::string, std::string>& assetsIdRedirections);

    static void deserializeRigidBodyComponent(
        kc::json::Node& componentDescription, ecs::Entity& entity,
        asset::AssetManager& assetManager,
        const std::unordered_map<std::string, std::string>& assetsIdRedirections);

    static math::Vec3 deserializeVector3(kc::json::Node& value);

    std::unordered_map<std::string, DeserializerCallback> m_deserializers;
    const std::unordered_map<std::string, std::string>& m_assetsIdRedirections;
};
}  // namespace sl::app