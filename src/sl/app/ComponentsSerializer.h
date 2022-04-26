#pragma once

#include <kc/json/Json.h>

#include <functional>
#include <unordered_map>

#include "sl/core/Misc.hpp"
#include "sl/ecs/Component.h"

namespace sl::app {

class ComponentsSerializer {
    using SerializeCallback = std::function<void(kc::json::JsonBuilder&, ecs::Component&)>;

   public:
    explicit ComponentsSerializer();

    void serializeComponent(
        std::type_index index, kc::json::JsonBuilder& builder, ecs::Component& component
    );

   private:
    static void serializeDirectionalLightComponent(
        kc::json::JsonBuilder& builder, ecs::Component& component
    );
    static void serializeMaterialComponent(
        kc::json::JsonBuilder& builder, ecs::Component& component
    );
    static void serializeMeshRendererComponent(
        kc::json::JsonBuilder& builder, ecs::Component& component
    );
    static void serializeModelComponent(kc::json::JsonBuilder& builder, ecs::Component& component);
    static void serializeParticleEffectComponent(
        kc::json::JsonBuilder& builder, ecs::Component& component
    );
    static void serializePointLightComponent(
        kc::json::JsonBuilder& builder, ecs::Component& component
    );
    static void serializeTransformComponent(
        kc::json::JsonBuilder& builder, ecs::Component& component
    );
    static void serializeRigidBodyComponent(
        kc::json::JsonBuilder& builder, ecs::Component& component
    );
    static void serializeVector(
        kc::json::JsonBuilder& builder, const std::string& name, const math::Vec3& vector
    );

    std::unordered_map<std::type_index, SerializeCallback> m_serializers;
};
}  // namespace sl::app