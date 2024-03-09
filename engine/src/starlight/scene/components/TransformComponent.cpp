#include "TransformComponent.h"

#include "starlight/scene/ecs/Entity.h"

namespace sl {

TransformComponent::TransformComponent(u64 id, u64 entityId) :
    Component(id, entityId) {}

TransformComponent::TransformComponent(
  u64 id, u64 entityId, const Vec3f& position, const Vec3f& scale,
  const Mat4f& rotation
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
    const auto position = getField<Vec3f>(root, "position");
    const auto scale    = getField<Vec3f>(root, "scale");
    const auto rotation = getField<Mat4f>(root, "rotation");

    entity->addComponent<TransformComponent>(position, scale, rotation);
}

}  // namespace sl
