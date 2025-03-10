#include "Entity.hh"

#include <fmt/core.h>

namespace sl {

Entity::Entity(ComponentManager& componentManager, std::optional<std::string> name) :
    NamedResource(name), m_componentManager(componentManager) {}

}  // namespace sl
