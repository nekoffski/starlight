#pragma once

#include <functional>
#include <unordered_map>

#include "sl/core/Json.h"
#include "sl/core/Misc.hpp"
#include "sl/ecs/Component.h"

namespace sl::app {

class ComponentsSerializer {
    using SerializeCallback = std::function<void(core::JsonBuilder&, ecs::Component&)>;

public:
    explicit ComponentsSerializer();

    void serializeComponent(std::type_index index, core::JsonBuilder& builder, ecs::Component& component);

private:
    static void serializeDirectionalLightComponent(core::JsonBuilder& builder, ecs::Component& component);
    static void serializeMaterialComponent(core::JsonBuilder& builder, ecs::Component& component);
    static void serializeMeshRendererComponent(core::JsonBuilder& builder, ecs::Component& component);
    static void serializeModelComponent(core::JsonBuilder& builder, ecs::Component& component);
    static void serializeParticleEffectComponent(core::JsonBuilder& builder, ecs::Component& component);
    static void serializePointLightComponent(core::JsonBuilder& builder, ecs::Component& component);
    static void serializeTransformComponent(core::JsonBuilder& builder, ecs::Component& component);
    static void serializeRigidBodyComponent(core::JsonBuilder& builder, ecs::Component& component);

    static void serializeVector(core::JsonBuilder& builder, const std::string& name, const math::Vec3& vector);

    std::unordered_map<std::type_index, SerializeCallback> m_serializers;
};
}