#include "Registry.hpp"

#include <kc/core/Log.h>

#include "Entity.h"

namespace sl::ecs {

Entity& Registry::createEntity(const std::string& name) {
    auto entity = std::make_unique<Entity>(name, *this);
    auto id = entity->getId();

    m_entities[id] = std::move(entity);
    m_entityNameToId[name] = id;

    auto& entityRef = *m_entities[id];

    LOG_INFO("Created entity: {}", entityRef.asString());

    return entityRef;
}

void Registry::updateComponentIndexes(const std::string& entityId, std::type_index componentIndex) {
    m_entities[entityId]->m_componentsIndexes.emplace_back(componentIndex);
}

void Registry::clear() {
    LOG_INFO("Cleaning up ecs registry");

    m_entities.clear();
    m_entityNameToId.clear();
    m_componentMap.clear();
}

void Registry::removeComponent(const std::string& entityId, std::type_index componentIndex) {
    m_componentMap.getByIndex(componentIndex).remove(entityId);
}

void Registry::removeEntity(const std::string& entityId) {
    auto entity = m_entities[entityId].get();

    LOG_INFO("Removing entity: {}", entity->asString());

    for (const auto& componentIndex : entity->m_componentsIndexes)
        removeComponent(entityId, componentIndex);

    m_entityNameToId.erase(entity->getName());
    m_entities.erase(entityId);
}

Entity& Registry::getEntityByName(const std::string& name) {
    return getEntityById(m_entityNameToId[name]);
}

Entity& Registry::getEntityById(const std::string& id) { return *m_entities[id]; }

bool Registry::hasEntityById(const std::string& id) const { return m_entities.contains(id); }

bool Registry::hasEntityByName(const std::string& name) const {
    return m_entityNameToId.contains(name);
}

const std::unordered_map<std::string, std::string>& Registry::getEntityNameToId() {
    return m_entityNameToId;
}

int Registry::getEntitiesCount() const { return m_entities.size(); }

std::unordered_map<std::string, std::unique_ptr<Entity>>& Registry::getEntities() {
    return m_entities;
}

}  // namespace sl::ecs
