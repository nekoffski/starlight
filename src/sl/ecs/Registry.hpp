#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "ComponentContainer.hpp"
#include "ComponentMap.hpp"
#include "ComponentView.hpp"
#include "sl/core/Logger.h"
#include "sl/core/Misc.hpp"

namespace sl::ecs {

class Entity;
class Component;

class Registry {
public:
    Entity& createEntity(const std::string&);

    template <typename T, typename... Args>
    T& addComponent(const std::string& entityId, Args&&... args) {
        auto& component = m_componentMap.get<T>().add(entityId, std::forward<Args>(args)...);
        component.ownerEntityId = entityId;

        updateComponentIndexes(entityId, core::typeIndex<T>());

        SL_INFO("Added {} to entity with id: {}", component.name, entityId);

        return component;
    }

    void removeEntity(const std::string& entityId);
    void clear();
    void removeComponent(const std::string& entityId, std::type_index componentIndex);

    template <typename T>
    T& getComponent(const std::string& entityId) {
        return m_componentMap.get<T>().getByEntityId(entityId);
    }

    Component& getComponentByIndex(const std::string& entityId, std::type_index index) {
        return m_componentMap.getByIndex(index).getByEntityId(entityId);
    }

    template <typename T>
    bool hasComponent(const std::string& entityId) {
        if (not m_componentMap.exists<T>())
            return false;
        return m_componentMap.get<T>().doesEntityOwnComponent(entityId);
    }

    template <typename T>
    ComponentView<T> getComponentView() {
        return ComponentView<T> { m_componentMap.get<T>() };
    }

    template <typename... T>
    auto getComponentsViews() {
        return std::make_tuple(getComponentView<T>()...); // TODO: investigate performance
    }

    Entity& getEntityByName(const std::string& name);
    Entity& getEntityById(const std::string& id);

    bool hasEntityById(const std::string& id) const;
    bool hasEntityByName(const std::string& name) const;

    const std::unordered_map<std::string, std::string>& getEntityNameToId();

    int getEntitiesCount() const;

    std::unordered_map<std::string, std::unique_ptr<Entity>>& getEntities();

private:
    void updateComponentIndexes(const std::string& entityId, std::type_index componentIndex);

    ComponentMap m_componentMap;
    std::unordered_map<std::string, std::unique_ptr<Entity>> m_entities;
    std::unordered_map<std::string, std::string> m_entityNameToId;
};
}
