#include "PointLightComponent.h"

namespace sl {

PointLightComponent::PointLightComponent(u64 id, u64 entityId) :
    Component(id, entityId) {}

kc::json::Node PointLightComponent::serialize() const {
    kc::json::Node root;
    root["component"] = "PointLightComponent";

    addJsonField(root, "position", light.position);
    addJsonField(root, "color", light.color);
    addJsonField(root, "attenuation", light.attenuationFactors);

    return root;
}

}  // namespace sl
