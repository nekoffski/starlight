#include "Entity.hh"

#include <fmt/core.h>

namespace sl {

Entity::Entity() : Entity(fmt::format("Entity_{}", getId())) {}

Entity::Entity(const std::string& name) : m_name(name) {}

}  // namespace sl