#include "Scene.hh"

#include "components/All.hh"

#include "starlight/resource/All.hh"

namespace sl {

Scene::Scene(u32 maxEntities) : m_entities("Entities", maxEntities) {
    // TODO: there is no need for that
    m_componentMap.registerContainer<MeshComponent>();
    m_componentMap.registerContainer<TransformComponent>();
    m_componentMap.registerContainer<MaterialComponent>();
    m_componentMap.registerContainer<PointLightComponent>();
}

Entity* Scene::addEntity(const std::string& name) {
    return storeEntity(m_entities.create(name, m_componentMap));
}
Entity* Scene::addEntity() { return storeEntity(m_entities.create(m_componentMap)); }

Entity* Scene::getEntity(const std::string& name) { return m_entitiesByName[name]; }
Entity* Scene::getEntity(u64 id) { return m_entities.get(id); }

RenderPacket Scene::getRenderPacket() {
    RenderPacket packet;
    packet.entities.reserve(128);

    m_componentMap.getComponents<MeshComponent>()->forEach(
      [&](MeshComponent* component) -> void {
          if (component->mesh == nullptr) [[unlikely]]
              return;

          auto entity = m_entities.get(component->entityId);

          auto worldTransform =
            entity->hasComponent<TransformComponent>()
              ? entity->getComponent<TransformComponent>()->transform.getModel()
              : identityMatrix;

          auto materialComponent = entity->getComponent<MaterialComponent>();
          auto material =
            (materialComponent && materialComponent->material)
              ? materialComponent->material
              : MaterialManager::get().getDefaultMaterial();

          packet.entities.emplace_back(worldTransform, component->mesh, material);
      }
    );

    // TODO:
    //  - get component count
    //  - for each should provide entity as well
    m_componentMap.getComponents<PointLightComponent>()->forEach(
      [&](PointLightComponent* component) -> void {
          auto light  = component->light;
          auto entity = m_entities.get(component->entityId);

          auto worldTransform =
            entity->hasComponent<TransformComponent>()
              ? entity->getComponent<TransformComponent>()->transform.getModel()
              : identityMatrix;

          light.position = worldTransform * light.position;
          packet.pointLights.push_back(light);
      }
    );

    return packet;
}

void Scene::clear() {
    m_entities.clear();
    m_entitiesByName.clear();
    m_componentMap.clear();
}

Entity* Scene::storeEntity(Entity* entity) {
    m_entitiesByName[entity->getName()] = entity;
    return entity;
}

}  // namespace sl
