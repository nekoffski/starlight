#include "Entity.hh"

namespace sl {

Entity::Entity(
  u64 id, const std::string& name, ComponentContainerMap& componentContainerMap
) :
    m_id(id),
    m_name(name), m_componentContainerMap(componentContainerMap) {}

Entity::Entity(u64 id, ComponentContainerMap& componentContainerMap) :
    Entity(id, fmt::format("Entity_{}", id), componentContainerMap) {}

const u64 Entity::getId() const { return m_id; }
const std::string& Entity::getName() const { return m_name; }

}  // namespace sl