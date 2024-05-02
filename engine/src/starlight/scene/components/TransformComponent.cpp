#include "TransformComponent.h"

#include "starlight/scene/ecs/Entity.h"

namespace sl {

TransformComponent::TransformComponent(u64 id, u64 entityId) :
    Component(id, entityId) {}

TransformComponent::TransformComponent(
  u64 id, u64 entityId, const Vec3<f32>& position, const Vec3<f32>& scale,
  const Mat4<f32>& rotation
) :
    Component(id, entityId),
    transform(position, scale, rotation) {}

kc::json::Node TransformComponent::serialize() const {
    kc::json::Node root;
    root["component"] = "TransformComponent";

    // TODO: handle parent transforms
    addJsonField(root, "position", transform.getPosition());
    addJsonField(root, "rotation", transform.getRotation());
    addJsonField(root, "scale", transform.getScale());

    return root;
}

void TransformComponent::deserialize(Entity* entity, const kc::json::Node& root) {
    const auto position = getField<Vec3<f32>>(root, "position");
    const auto scale    = getField<Vec3<f32>>(root, "scale");
    const auto rotation = getField<Mat4<f32>>(root, "rotation");

    entity->addComponent<TransformComponent>(position, scale, rotation);
}

}  // namespace sl
