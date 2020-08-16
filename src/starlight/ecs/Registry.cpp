#include "Registry.h"

#include <crossguid/guid.hpp>

#include "Entity.h"

namespace sl::ecs {

std::shared_ptr<Entity> Registry::createEntity() {
    auto id = xg::newGuid().str();
    auto entity = std::make_shared<Entity>(id, *this);
    m_entities[std::move(id)] = entity;
    return entity;
}
}