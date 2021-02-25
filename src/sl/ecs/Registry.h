#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "ComponentContainer.hpp"
#include "ComponentMap.hpp"
#include "ComponentView.hpp"
#include "sl/core/Logger.h"
#include "sl/core/misc/misc.hpp"

namespace sl::ecs {

class Entity;
class Component;

class Registry {
public:
    std::shared_ptr<Entity> createEntity(const std::string&);

    template <typename T, typename... Args>
    void addComponent(const std::string& entityId, Args&&... args) {
        auto& component = m_componentMap.get<T>()->add(entityId, std::forward<Args>(args)...);
        component.ownerEntityId = entityId;
    }

    template <typename T>
    T& getComponent(const std::string& entityId) {
        return m_componentMap.get<T>()->getByEntityId(entityId);
    }

    Component& getComponentByIndex(const std::string& entityId, std::type_index index) {
        return m_componentMap.getByIndex<Component>(index)->getByEntityId(entityId);
    }

    template <typename T>
    bool hasComponent(const std::string& entityId) {
        if (not m_componentMap.exists<T>())
            return false;
        return m_componentMap.get<T>()->doesEntityOwnComponent(entityId);
    }

    template <typename T>
    ComponentView<T> getComponentView() {
        return ComponentView<T>{ m_componentMap.get<T>() };
    }

    std::shared_ptr<Entity> getEntityByName(const std::string& name) {
        return getEntityById(m_entityNameToId[name]);
    }

    std::shared_ptr<Entity> getEntityById(const std::string& id) {
        return m_entities[id];
    }

    const std::unordered_map<std::string, std::string>& getEntityNameToId() {
        return m_entityNameToId;
    }

    std::unordered_map<std::string, std::shared_ptr<Entity>>& getEntities() {
        return m_entities;
    }

private:
    ComponentMap m_componentMap;
    std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
    std::unordered_map<std::string, std::string> m_entityNameToId;
};
}
