#include "TransformComponent.h"

namespace sl {

TransformComponent::TransformComponent(u64 id, u64 entityId) :
    Component(id, entityId) {}

kc::json::Node TransformComponent::serialize() const {
    kc::json::Node root;
    root["component"] = "TransformComponent";

    // TODO: handle parent transforms
    addJsonField(root, "position", transform.getPosition());
    addJsonField(root, "orientation", transform.getRotation());
    addJsonField(root, "scale", transform.getScale());

    return root;
}

}  // namespace sl
