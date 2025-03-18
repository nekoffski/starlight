#include "Component.hh"

#include "Entity.hh"

namespace sl {

ComponentBase::ComponentBase(Entity& entity) : m_entity(entity) {}

u64 ComponentBase::getEntityId() const { return m_entity.id; }

}  // namespace sl
