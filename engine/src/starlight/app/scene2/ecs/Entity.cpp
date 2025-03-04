#include "Entity.hh"

#include "EntityManager.hh"

namespace sl::v2 {

Entity::Entity(EntityManager& entityManager, OptStr name) :
    NamedResource(name), m_entityManager(entityManager) {}

}  // namespace sl::v2
