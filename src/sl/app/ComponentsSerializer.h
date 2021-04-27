#pragma once

#include <functional>
#include <unordered_map>

#include "sl/core/Misc.hpp"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

#define BIND_SERIALIZER_CALLBACK(Component) \
    { sl::core::typeIndex<sl::scene::components::Component>(), &sl::app::ComponentsSerializer::serialize##Component }

namespace sl::app {

class ComponentsSerializer {
    using SerializeCallback = std::function<void(core::JsonBuilder&, ecs::Component&)>;

public:
    explicit ComponentsSerializer() {
        m_serializers = {
            BIND_SERIALIZER_CALLBACK(DirectionalLightComponent),
            BIND_SERIALIZER_CALLBACK(MaterialComponent),
            BIND_SERIALIZER_CALLBACK(MeshRendererComponent),
            BIND_SERIALIZER_CALLBACK(ModelComponent),
            BIND_SERIALIZER_CALLBACK(ParticleEffectComponent),
            BIND_SERIALIZER_CALLBACK(PointLightComponent),
            BIND_SERIALIZER_CALLBACK(TransformComponent)
        };
    }

    void serializeComponent(std::type_index index, core::JsonBuilder& builder, ecs::Component& component) {
        if (m_serializers.contains(index))
            m_serializers.at(index)(builder, component);
    }

private:
    static void serializeDirectionalLightComponent(core::JsonBuilder& builder, ecs::Component& component) {
        auto& directionalLightComponent = static_cast<scene::components::DirectionalLightComponent&>(component);

        builder.addField("name", "DirectionalLightComponent"s);
        serializeVector(builder, "direction", directionalLightComponent.direction);
        serializeVector(builder, "color", directionalLightComponent.color);
    }

    static void serializeMaterialComponent(core::JsonBuilder& builder, ecs::Component& component) {
        auto& materialComponent = static_cast<scene::components::MaterialComponent&>(component);

        builder.addField("name", "MaterialComponent"s);
        serializeVector(builder, "ambient-color", materialComponent.ambientColor);
        serializeVector(builder, "diffuse-color", materialComponent.diffuseColor);
        serializeVector(builder, "specular-color", materialComponent.specularColor);
        builder.addField("shininess", materialComponent.shininess);
    }

    static void serializeMeshRendererComponent(core::JsonBuilder& builder, ecs::Component& component) {
        auto& meshRendererComponent = static_cast<scene::components::MeshRendererComponent&>(component);

        builder
            .addField("name", "MeshRendererComponent"s)
            .addField("shader-id", meshRendererComponent.shader->id);
    }

    static void serializeModelComponent(core::JsonBuilder& builder, ecs::Component& component) {
        auto& modelComponent = static_cast<scene::components::ModelComponent&>(component);

        std::vector<int> meshesIds;
        std::ranges::transform(modelComponent.meshes, std::back_inserter(meshesIds),
            [](auto& mesh) -> int { return mesh->id; });

        builder.addField("name", "ModelComponent"s).addField("meshes-ids", meshesIds);
    }

    static void serializeParticleEffectComponent(core::JsonBuilder& builder, ecs::Component& component) {
        auto& particleEffectComponent = static_cast<scene::components::ParticleEffectComponent&>(component);
    }

    static void serializePointLightComponent(core::JsonBuilder& builder, ecs::Component& component) {
        auto& pointLightComponent = static_cast<scene::components::PointLightComponent&>(component);

        builder.addField("name", "PointLightComponent"s);
        serializeVector(builder, "position", pointLightComponent.position);
        serializeVector(builder, "color", pointLightComponent.color);
        builder
            .beginObject("attenuation")
            .addField("a", pointLightComponent.attenuationA)
            .addField("b", pointLightComponent.attenuationB)
            .addField("c", pointLightComponent.attenuationC)
            .endObject();
    }

    static void serializeTransformComponent(core::JsonBuilder& builder, ecs::Component& component) {
        auto& transformComponent = static_cast<scene::components::TransformComponent&>(component);

        builder.addField("name", "TransformComponent"s);
        serializeVector(builder, "position", transformComponent.position);
        serializeVector(builder, "rotation", transformComponent.rotation);
        serializeVector(builder, "scale", transformComponent.scale);
    }

    static void serializeVector(core::JsonBuilder& builder, const std::string& name, const math::Vec3& vector) {
        builder.addField(name, std::vector<float> { vector.x, vector.y, vector.z });
    }

    std::unordered_map<std::type_index, SerializeCallback> m_serializers;
};
}