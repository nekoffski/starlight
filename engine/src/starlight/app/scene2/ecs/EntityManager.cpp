#include "EntityManager.hh"

namespace sl::v2 {

Entity& EntityManager::addEntity(OptStr name) {
    return m_entities.emplace(*this, name);
}

bool EntityManager::hasEntity(const std::string& name) {
    return m_entities.contains(name);
}

Entity* EntityManager::getEntity(const std::string& name) {
    return m_entities.find(name);
}

}  // namespace sl::v2
