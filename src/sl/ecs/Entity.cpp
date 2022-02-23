#include "Entity.h"

namespace sl::ecs {

Entity::Entity(const std::string& name, Registry& registry)
    : isActive(true), m_name(name), m_registry(registry) {}

void Entity::setName(const std::string& name) { m_name = name; }

std::string Entity::asString() const {
    return fmt::format("[Entity name: {}, id: {}]", m_name, m_id);
}

std::string Entity::getName() const { return m_name; }

void Entity::removeComponent(std::type_index index) {
    std::erase(m_componentsIndexes, index);
    m_registry.removeComponent(m_id, index);
}

std::vector<std::type_index> Entity::getComponentsIndexes() { return m_componentsIndexes; }

Component& Entity::getComponent(std::type_index index) {
    return m_registry.getComponentByIndex(m_id, index);
}

}  // namespace sl::ecs
