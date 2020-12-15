#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "ComponentContainer.hpp"
#include "ComponentWrapper.h"
#include "ComponentsMap.hpp"
#include "sl/core/log/Logger.h"

namespace sl::ecs {

class Entity;
class Component;

class Registry {
public:
    std::shared_ptr<Entity> createEntity(std::string);

    template <typename Component, typename... Args>
    void addComponent(std::string entityId, Args&&... args) {
        auto& component = m_componentsMap.get<Component>()->add(entityId, std::forward<Args>(args)...);
        component.entity = m_entities[std::move(entityId)];
        component.entity->m_componentsIndexes.push_back(TYPE_INDEX(Component));
    }

    template <typename Component, typename Factory>
    void setComponentWrapperFactory() {
        m_componentToWrapperFactory[TYPE_INDEX(Component)] =
            std::make_unique<Factory>();
    }

    std::unique_ptr<ComponentWrapper> wrapComponentByIndex(std::type_index index, std::string id) {
        auto& component = m_componentsMap.getByIndex<Component>(index)->getByEntityId(id);
        if (m_componentToWrapperFactory.count(index) == 0)
            return std::make_unique<NullComponentWrapper>(component);
        auto& factory = m_componentToWrapperFactory[index];
        return factory->create(component);
    }

    template <typename Component>
    Component& getComponent(std::string entityId) {
        return m_componentsMap.get<Component>()->getByEntityId(std::move(entityId));
    }

    template <typename Component>
    bool hasComponent(std::string entityId) {
        if (not m_componentsMap.exists<Component>())
            return false;
        return m_componentsMap.get<Component>()->isEntityOwner(std::move(entityId));
    }

    template <typename Component>
    std::vector<Component>& getComponentView() {
        return m_componentsMap.get<Component>()->getAll();
    }

    std::shared_ptr<Entity> getEntityByName(std::string name) {
        return getEntityById(m_entityNameToId[std::move(name)]);
    }

    std::shared_ptr<Entity> getEntityById(std::string id) {
        return m_entities[std::move(id)];
    }

    const std::unordered_map<std::string, std::string>& getEntityNameToId() {
        return m_entityNameToId;
    }

private:
    ComponentsMap<IComponentContainer, ComponentContainer> m_componentsMap;
    std::unordered_map<std::string, std::shared_ptr<Entity>> m_entities;
    std::unordered_map<std::string, std::string> m_entityNameToId;
    std::unordered_map<std::type_index, std::unique_ptr<ComponentWrapperFactory>> m_componentToWrapperFactory;
};
}
