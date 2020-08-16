#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "ComponentContainer.hpp"
#include "ComponentsMap.hpp"
#include "starlight/core/log/Logger.h"

namespace sl::ecs {

class Entity;

class Registry {
public:
    std::shared_ptr<Entity> createEntity();

    template <typename Component, typename... Args>
    void addComponent(std::string entityId, Args&&... args) {
        m_componentsMap.get<Component>()->add(std::move(entityId), std::forward<Args>(args)...);
    }

    template <typename Component>
    Component& getComponent(std::string entityId) {
        return m_componentsMap.get<Component>()->getByEntityId(std::move(entityId));
    }

    template <typename Component>
    bool hasComponent(std::string entityId) {
        if (not m_componentsMap.exists<Component>())
            return false;
        return m_componentsMap.get<Component>().isEntityOwner(std::move(entityId));
    }

    template <typename Component>
    std::vector<Component>& getComponentView() {
        return m_componentsMap.get<Component>()->getAll();
    }

private:
    ComponentsMap<IComponentContainer, ComponentContainer> m_componentsMap;
    std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
};
}