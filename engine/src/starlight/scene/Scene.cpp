#include "Scene.h"

#include "components/MeshComponent.h"
#include "components/TransformComponent.h"

namespace sl {

Scene::Scene(u32 maxEntities) : m_entities("Entities", maxEntities) {
    m_componentMap.registerContainer<MeshComponent>();
    m_componentMap.registerContainer<TransformComponent>();
}

Entity* Scene::addEntity(const std::string& name) {
    return storeEntity(m_entities.create(name, m_componentMap));
}
Entity* Scene::addEntity() { return storeEntity(m_entities.create(m_componentMap)); }

Entity* Scene::getEntity(const std::string& name) { return m_entitiesByName[name]; }
Entity* Scene::getEntity(u64 id) { return m_entities.get(id); }

RenderPacket Scene::getRenderPacket() {
    RenderPacket packet{};
    packet.meshes.reserve(128);

    m_componentMap.getComponents<MeshComponent>()->forEach(
      [&](MeshComponent* component) -> void {
          auto mesh   = component->mesh;
          auto entity = m_entities.get(component->entityId);
          if (entity->hasComponent<TransformComponent>()) {
              // TODO: consider binding it once on adding Transform/Mesh components
              auto& transform =
                entity->getComponent<TransformComponent>()->transform;
              mesh->transform = &transform;
          }
          packet.meshes.push_back(mesh);
      }
    );
    return packet;
}

Entity* Scene::storeEntity(Entity* entity) {
    m_entitiesByName[entity->getName()] = entity;
    return entity;
}

}  // namespace sl
