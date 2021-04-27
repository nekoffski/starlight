#include "Entity.h"

namespace sl::ecs {

Entity::Entity(const std::string& id, const std::string& name, Registry& registry)
    : isActive(true)
    , m_id(id)
    , m_name(name)
    , m_registry(registry) {
}

std::string Entity::getId() const {
    return m_id;
}

std::string Entity::getName() const {
    return m_name;
}

void Entity::removeComponent(std::type_index index) {
    std::erase(m_componentsIndexes, index);
    m_registry.removeComponent(m_id, index);
}

std::vector<std::type_index> Entity::getComponentsIndexes() {
    return m_componentsIndexes;
}

Component& Entity::getComponent(std::type_index index) {
    return m_registry.getComponentByIndex(m_id, index);
}

}
