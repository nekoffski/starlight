#include "ComponentsDeserializer.h"

#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

#include "sl/physx/AxisAlignedBoundingBox.h"
#include "sl/utils/Globals.h"

using namespace sl::scene::components;

#define BIND_DESERIALIZER_CALLBACK(ComponentName) \
    { #ComponentName, &sl::app::ComponentsDeserializer::deserialize##ComponentName }

namespace sl::app {

ComponentsDeserializer::ComponentsDeserializer() {
    m_deserializers = {
        BIND_DESERIALIZER_CALLBACK(DirectionalLightComponent),
        BIND_DESERIALIZER_CALLBACK(MaterialComponent),
        BIND_DESERIALIZER_CALLBACK(MeshRendererComponent),
        BIND_DESERIALIZER_CALLBACK(ModelComponent),
        BIND_DESERIALIZER_CALLBACK(ParticleEffectComponent),
        BIND_DESERIALIZER_CALLBACK(PointLightComponent),
        BIND_DESERIALIZER_CALLBACK(TransformComponent),
        BIND_DESERIALIZER_CALLBACK(RigidBodyComponent)
    };
}

void ComponentsDeserializer::deserializeComponent(const std::string& name, Json::Value& componentDescription,
    ecs::Entity& entity, asset::AssetManager& assetManager) {

    if (not m_deserializers.contains(name))
        return;

    m_deserializers.at(name)(componentDescription, entity, assetManager);
}

void ComponentsDeserializer::deserializeDirectionalLightComponent(Json::Value& componentDescription,
    ecs::Entity& entity, asset::AssetManager& assetManager) {

    auto& component = entity.addComponent<DirectionalLightComponent>(
        deserializeVector3(componentDescription["direction"]),
        deserializeVector3(componentDescription["color"]));

    component.isActive = componentDescription["active"].asBool();
    component.renderDirection = componentDescription["render-direction"].asBool();
}

void ComponentsDeserializer::deserializeMaterialComponent(Json::Value& componentDescription,
    ecs::Entity& entity, asset::AssetManager& assetManager) {

    auto& component = entity.addComponent<MaterialComponent>(
        deserializeVector3(componentDescription["ambient-color"]),
        deserializeVector3(componentDescription["diffuse-color"]),
        deserializeVector3(componentDescription["specular-color"]),
        componentDescription["shininess"].asFloat());

    component.isActive = componentDescription["active"].asBool();
}

void ComponentsDeserializer::deserializeMeshRendererComponent(Json::Value& componentDescription,
    ecs::Entity& entity, asset::AssetManager& assetManager) {
    auto& component = entity.addComponent<MeshRendererComponent>();

    auto& defaultShader = GLOBALS().shaders->defaultModelShader;

    if (defaultShader->id == componentDescription["shader-id"].asInt())
        component.shader = defaultShader;

    component.isActive = componentDescription["active"].asBool();
}

void ComponentsDeserializer::deserializeModelComponent(Json::Value& componentDescription,
    ecs::Entity& entity, asset::AssetManager& assetManager) {

    auto& component = entity.addComponent<ModelComponent>();
    auto& meshes = assetManager.getMeshes();

    for (auto& meshId : componentDescription["meshes-ids"]) {
        auto id = meshId.asInt();

        auto mesh = [&]() {
            if (meshes.has(id))
                return meshes.getById(id);

            for (auto& mesh : GLOBALS().geom->meshes | std::views::values)
                if (mesh->id == id)
                    return mesh;

            // TODO: handle this case
            throw std::runtime_error { "Unhandled case" };
        }();

        component.meshes.push_back(mesh);
    }

    component.isActive = componentDescription["active"].asBool();
}

void ComponentsDeserializer::deserializeParticleEffectComponent(Json::Value& componentDescription,
    ecs::Entity& entity, asset::AssetManager& assetManager) {
}

void ComponentsDeserializer::deserializePointLightComponent(Json::Value& componentDescription,
    ecs::Entity& entity, asset::AssetManager& assetManager) {

    auto& attenuation = componentDescription["attenuation"];
    auto& component = entity.addComponent<PointLightComponent>(
        deserializeVector3(componentDescription["position"]),
        deserializeVector3(componentDescription["color"]),
        attenuation["a"].asFloat(),
        attenuation["b"].asFloat(),
        attenuation["c"].asFloat());

    component.isActive = componentDescription["active"].asBool();
}

void ComponentsDeserializer::deserializeTransformComponent(Json::Value& componentDescription,
    ecs::Entity& entity, asset::AssetManager& assetManager) {

    auto& component = entity.addComponent<TransformComponent>(
        deserializeVector3(componentDescription["position"]),
        deserializeVector3(componentDescription["rotation"]),
        deserializeVector3(componentDescription["scale"]));

    component.isActive = componentDescription["active"].asBool();
}

void ComponentsDeserializer::deserializeRigidBodyComponent(Json::Value& componentDescription,
    ecs::Entity& entity, asset::AssetManager& assetManager) {

    auto& component = entity.addComponent<RigidBodyComponent>();

    component.useGravity = componentDescription["use-gravity"].asBool();
    component.renderBoundingBox = componentDescription["render-bounding-box"].asBool();
    component.enableCollisions = componentDescription["enable-collisions"].asBool();
    component.fixed = componentDescription["fixed"].asBool();
    component.mass = componentDescription["mass"].asFloat();
    component.velocity = deserializeVector3(componentDescription["velocity"]);

    auto boundingBoxName = componentDescription["bounding-box"].asString();

    component.boundingBox = [&]() -> std::unique_ptr<physx::BoundingBox> {
        if (not entity.hasComponent<ModelComponent>())
            return nullptr;

        auto meshes = entity.getComponent<ModelComponent>().meshes;

        if (boundingBoxName == "AxisAlignedBoundingBox")
            return std::make_unique<physx::AxisAlignedBoundingBox>(meshes);

        return nullptr;
    }();

    component.isActive = componentDescription["active"].asBool();
}

math::Vec3 ComponentsDeserializer::deserializeVector3(Json::Value& value) {
    return math::Vec3 {
        value[0].asFloat(), value[1].asFloat(), value[2].asFloat()
    };
}

}