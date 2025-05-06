#include "Component.hh"
#include "Component.hh"

#include "Entity.hh"

namespace sl {

Component::Component(Entity& entity)
    : m_entity(entity) {}

u64 Component::getEntityId() const { return m_entity.getId(); }

Entity& Component::getEntity() { return m_entity; }

}  // namespace sl
