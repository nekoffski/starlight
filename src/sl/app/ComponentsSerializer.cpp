#include "ComponentsSerializer.h"

#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

#define BIND_SERIALIZER_CALLBACK(Component)                      \
    {                                                            \
        sl::core::typeIndex<sl::scene::components::Component>(), \
            &sl::app::ComponentsSerializer::serialize##Component \
    }

namespace sl::app {

ComponentsSerializer::ComponentsSerializer() {
    m_serializers = {BIND_SERIALIZER_CALLBACK(DirectionalLightComponent),
                     BIND_SERIALIZER_CALLBACK(MaterialComponent),
                     BIND_SERIALIZER_CALLBACK(MeshRendererComponent),
                     BIND_SERIALIZER_CALLBACK(ModelComponent),
                     BIND_SERIALIZER_CALLBACK(ParticleEffectComponent),
                     BIND_SERIALIZER_CALLBACK(PointLightComponent),
                     BIND_SERIALIZER_CALLBACK(TransformComponent),
                     BIND_SERIALIZER_CALLBACK(RigidBodyComponent)};
}

void ComponentsSerializer::serializeComponent(std::type_index index, kc::json::JsonBuilder& builder,
                                              ecs::Component& component) {
    if (m_serializers.contains(index)) std::invoke(m_serializers.at(index), builder, component);
}

void ComponentsSerializer::serializeDirectionalLightComponent(kc::json::JsonBuilder& builder,
                                                              ecs::Component& component) {
    auto& directionalLightComponent =
        static_cast<scene::components::DirectionalLightComponent&>(component);

    builder.addField("name", "DirectionalLightComponent"s)
        .addField("active", directionalLightComponent.isActive)
        .addField("render-direction", directionalLightComponent.renderDirection);

    serializeVector(builder, "direction", directionalLightComponent.direction);
    serializeVector(builder, "color", directionalLightComponent.color);
}

void ComponentsSerializer::serializeMaterialComponent(kc::json::JsonBuilder& builder,
                                                      ecs::Component& component) {
    auto& materialComponent = static_cast<scene::components::MaterialComponent&>(component);

    builder.addField("name", "MaterialComponent"s).addField("active", materialComponent.isActive);
    serializeVector(builder, "ambient-color", materialComponent.ambientColor);
    serializeVector(builder, "diffuse-color", materialComponent.diffuseColor);
    serializeVector(builder, "specular-color", materialComponent.specularColor);
    builder.addField("shininess", materialComponent.shininess);
}

void ComponentsSerializer::serializeMeshRendererComponent(kc::json::JsonBuilder& builder,
                                                          ecs::Component& component) {
    auto& meshRendererComponent = static_cast<scene::components::MeshRendererComponent&>(component);

    builder.addField("name", "MeshRendererComponent"s)
        .addField("shader-id", meshRendererComponent.shader->getId())
        .addField("active", meshRendererComponent.isActive);
}

void ComponentsSerializer::serializeModelComponent(kc::json::JsonBuilder& builder,
                                                   ecs::Component& component) {
    auto& modelComponent = static_cast<scene::components::ModelComponent&>(component);

    std::vector<std::string> meshesIds;
    std::ranges::transform(modelComponent.meshes, std::back_inserter(meshesIds),
                           [](auto& mesh) -> std::string { return mesh->getId(); });

    const auto boundingBox =
        modelComponent.boundingBox ? modelComponent.boundingBox->getName() : "None";

    builder.addField("name", "ModelComponent"s)
        .addField("meshes-ids", meshesIds)
        .addField("active", modelComponent.isActive)
        .addField("render-bounding-box", modelComponent.renderBoundingBox)
        .addField("bounding-box", boundingBox);
}

void ComponentsSerializer::serializeParticleEffectComponent(kc::json::JsonBuilder& builder,
                                                            ecs::Component& component) {
    auto& particleEffectComponent =
        static_cast<scene::components::ParticleEffectComponent&>(component);
    auto& settings = particleEffectComponent.pfxGeneratorSettings;

    builder.addField("name", "ParticleEffectComponent"s)
        .addField("is-active", component.isActive)
        .addField("max-particles", particleEffectComponent.maxParticles)
        .addField("min-scale", settings.minScale)
        .addField("max-scale", settings.maxScale)
        .addField("min-speed", settings.minSpeed)
        .addField("max-speed", settings.maxSpeed)
        .addField("radius", settings.radius)
        .addField("direction-factor", settings.directionFactor)
        .addField("delta-scale", settings.deltaScale);

    serializeVector(builder, "min-color", settings.minColor);
    serializeVector(builder, "max-color", settings.maxColor);
    serializeVector(builder, "position", particleEffectComponent.position);
}

void ComponentsSerializer::serializePointLightComponent(kc::json::JsonBuilder& builder,
                                                        ecs::Component& component) {
    auto& pointLightComponent = static_cast<scene::components::PointLightComponent&>(component);

    builder.addField("name", "PointLightComponent"s)
        .addField("active", pointLightComponent.isActive);
    serializeVector(builder, "position", pointLightComponent.position);
    serializeVector(builder, "color", pointLightComponent.color);
    builder.beginObject("attenuation")
        .addField("a", pointLightComponent.attenuationA)
        .addField("b", pointLightComponent.attenuationB)
        .addField("c", pointLightComponent.attenuationC)
        .endObject();
}

void ComponentsSerializer::serializeTransformComponent(kc::json::JsonBuilder& builder,
                                                       ecs::Component& component) {
    auto& transformComponent = static_cast<scene::components::TransformComponent&>(component);

    builder.addField("name", "TransformComponent"s).addField("active", transformComponent.isActive);
    serializeVector(builder, "position", transformComponent.position);
    serializeVector(builder, "rotation", transformComponent.rotation);
    serializeVector(builder, "scale", transformComponent.scale);
}

void ComponentsSerializer::serializeRigidBodyComponent(kc::json::JsonBuilder& builder,
                                                       ecs::Component& component) {
    auto& rigidBodyComponent = static_cast<scene::components::RigidBodyComponent&>(component);

    builder.addField("name", "RigidBodyComponent"s)
        .addField("use-gravity", rigidBodyComponent.useGravity)
        .addField("mass", rigidBodyComponent.mass)
        .addField("enable-collisions", rigidBodyComponent.enableCollisions)
        .addField("fixed", rigidBodyComponent.fixed)
        .addField("active", rigidBodyComponent.isActive);

    serializeVector(builder, "velocity", rigidBodyComponent.velocity);
}

void ComponentsSerializer::serializeVector(kc::json::JsonBuilder& builder, const std::string& name,
                                           const math::Vec3& vector) {
    builder.addField(name, std::vector<float>{vector.x, vector.y, vector.z});
}
}  // namespace sl::app