#include "MaterialComponent.h"

namespace sl {

MaterialComponent::MaterialComponent(u64 id, u64 entityId, Material* material) :
    Component(id, entityId), material(material) {}

kc::json::Node MaterialComponent::serialize() const {
    kc::json::Node root;
    root["component"] = "MaterialComponent";
    if (material) root["name"] = material->getName();
    return root;
}

}  // namespace sl
