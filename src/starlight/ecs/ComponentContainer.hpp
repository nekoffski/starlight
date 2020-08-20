#pragma once

#include <string>
#include <unordered_map>
#include <vector>

static constexpr int CONTAINER_SIZE = 1000;

namespace sl::ecs {

class IComponentContainer {
public:
    virtual ~IComponentContainer() = default;
};

// TODO: FIX REMOVING COMPONENTS!
// TODO: add CONCEPTS

template <typename Component, template <typename> typename Container = std::vector>
class ComponentContainer : public IComponentContainer {
public:
    static std::shared_ptr<IComponentContainer> create() {
        return std::make_shared<ComponentContainer<Component>>();
    }

    explicit ComponentContainer() {
        m_components.reserve(CONTAINER_SIZE);
    }

    template <typename... Args>
    Component& add(std::string entityId, Args&&... args) {
        m_components.emplace_back(Component(std::forward<Args>(args)...));
        auto& component = m_components.back();
        m_entityIdToComponent[std::move(entityId)] = &component;
        return component;
    }

    Container<Component>& getAll() {
        return m_components;
    }

    Component& getByEntityId(std::string entityId) {
        return *m_entityIdToComponent[std::move(entityId)];
    }

    bool isEntityOwner(std::string entityId) {
        return m_entityIdToComponent.count(std::move(entityId)) > 0;
    }

private:
    Container<Component> m_components; // this shoud be possible replaced with array as vector realocates memory every push
    std::unordered_map<std::string, Component*> m_entityIdToComponent;
};
}