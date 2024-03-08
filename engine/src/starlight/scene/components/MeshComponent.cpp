#include "MeshComponent.h"

namespace sl {

MeshComponent::MeshComponent(u64 id, u64 entityId, Mesh* mesh) :
    Component(id, entityId), mesh(mesh) {}

kc::json::Node MeshComponent::serialize() const {
    kc::json::Node root;
    root["component"] = "MeshComponent";
    if (mesh) root["name"] = mesh->getProperties().name;
    return root;
}

}  // namespace sl
