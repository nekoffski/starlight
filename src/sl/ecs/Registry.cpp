#include "Registry.h"

#include <crossguid/guid.hpp>

#include "Entity.h"

namespace sl::ecs {

std::shared_ptr<Entity> Registry::createEntity(const std::string& name) {
    auto entity = std::make_shared<Entity>(name, *this);
    auto id = entity->getId();

    m_entities[id] = entity;
    m_entityNameToId[name] = id;
    return entity;
}

void Registry::removeEntity(const std::string& entityId) {
    auto entity = m_entities[entityId];

    for (const auto& componentIndex : entity->m_componentsIndexes)
        removeComponent(entityId, componentIndex);

    m_entityNameToId.erase(entity->getName());
    m_entities.erase(entityId);
}
}
