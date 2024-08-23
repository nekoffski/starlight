#include "MaterialComponent.hh"

#include "starlight/scene/ecs/Entity.hh"
#include "starlight/resource/managers/MaterialManager.hh"

namespace sl {

MaterialComponent::MaterialComponent(u64 id, u64 entityId, Material* material) :
    Component(id, entityId), material(material) {}

kc::json::Node MaterialComponent::serialize() const {
    kc::json::Node root;
    root["component"] = "MaterialComponent";
    if (material) root["name"] = material->getName();
    return root;
}

void MaterialComponent::deserialize(Entity* entity, const kc::json::Node& root) {
    if (root.isMember("name")) {
        const auto materialName = getField<std::string>(root, "name");
        const auto material     = MaterialManager::get().acquire(materialName);
        entity->addComponent<MaterialComponent>(material);
    } else {
        entity->addComponent<MaterialComponent>();
    }
}

}  // namespace sl
