#include "PointLightComponent.hh"

#include "starlight/scene/ecs/Entity.hh"

namespace sl {

PointLightComponent::PointLightComponent(u64 id, u64 entityId) :
    Component(id, entityId) {}

PointLightComponent::PointLightComponent(
  u64 id, u64 entityId, const Vec4<f32>& position, const Vec4<f32>& color,
  const Vec4<f32>& attenuation
) :
    Component(id, entityId),
    light{ position, color, attenuation } {}

kc::json::Node PointLightComponent::serialize() const {
    kc::json::Node root;
    root["component"] = "PointLightComponent";

    addJsonField(root, "position", light.position);
    addJsonField(root, "color", light.color);
    addJsonField(root, "attenuation", light.attenuationFactors);

    return root;
}

void PointLightComponent::deserialize(Entity* entity, const kc::json::Node& root) {
    const auto position    = getField<Vec4<f32>>(root, "position");
    const auto color       = getField<Vec4<f32>>(root, "color");
    const auto attenuation = getField<Vec4<f32>>(root, "attenuation");

    entity->addComponent<PointLightComponent>(position, color, attenuation);
}

}  // namespace sl
