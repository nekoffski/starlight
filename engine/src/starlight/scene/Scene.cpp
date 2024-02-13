#include "Scene.h"

#include "components/ModelComponent.h"
#include "components/TransformComponent.h"

namespace sl {

Scene::Scene(u32 maxEntities) : m_entities("Entities", maxEntities) {
    m_componentMap.registerContainer<ModelComponent>();
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
    packet.models.reserve(128);

    m_componentMap.getComponents<ModelComponent>()->forEach(
      [&](ModelComponent* component) -> void {
          auto model  = component->model;
          auto entity = m_entities.get(component->entityId);
          if (entity->hasComponent<TransformComponent>()) {
              // TODO: consider binding it once on adding Transform/Model components
              auto& transform =
                entity->getComponent<TransformComponent>()->transform;
              model->setTransform(&transform);
          }
          packet.models.push_back(model);
      }
    );
    return packet;
}

Entity* Scene::storeEntity(Entity* entity) {
    m_entitiesByName[entity->getName()] = entity;
    return entity;
}

}  // namespace sl
