#include "MeshComponent.h"

#include "starlight/scene/ecs/Entity.h"

#include "starlight/resource/managers/MeshManager.h"

namespace sl {

MeshComponent::MeshComponent(u64 id, u64 entityId, Mesh* mesh) :
    Component(id, entityId), mesh(mesh) {}

kc::json::Node MeshComponent::serialize() const {
    kc::json::Node root;
    root["component"] = "MeshComponent";
    if (mesh) root["name"] = mesh->getProperties().name;
    return root;
}

void MeshComponent::deserialize(Entity* entity, const kc::json::Node& root) {
    if (root.isMember("name")) {
        const auto meshName = getField<std::string>(root, "name");
        const auto mesh     = MeshManager::get().acquire(meshName);
        entity->addComponent<MeshComponent>(mesh);
    } else {
        entity->addComponent<MeshComponent>();
    }
}

}  // namespace sl
