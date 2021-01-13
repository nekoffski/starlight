#include "Registry.h"

#include <crossguid/guid.hpp>

#include "Entity.h"

namespace sl::ecs {

std::shared_ptr<Entity> Registry::createEntity(const std::string& name) {
    auto id = xg::newGuid().str();
    auto entity = std::make_shared<Entity>(id, name, *this);
    m_entities[id] = entity;
    m_entityNameToId[name] = id;
    return entity;
}
}
