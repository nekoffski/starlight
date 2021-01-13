#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

static constexpr int defaultContainerSize = 1000;

namespace sl::ecs {

class IComponentContainer {
public:
    virtual ~IComponentContainer() = default;
};

// TODO: FIX REMOVING COMPONENTS!
// TODO: add CONCEPTS

template <typename T>
class ComponentContainer : public IComponentContainer {
public:
    static std::shared_ptr<IComponentContainer> create() {
        return std::make_shared<ComponentContainer<T>>();
    }

    explicit ComponentContainer() {
        m_components.reserve(defaultContainerSize);
    }

    template <typename... Args>
    T& add(const std::string& entityId, Args&&... args) {
        m_components.emplace_back(T(std::forward<Args>(args)...));
        auto& component = m_components.back();
        m_entityIdToComponent[entityId] = &component;
        return component;
    }

    std::vector<T>& getAll() {
        return m_components;
    }

    T& getByEntityId(const std::string& entityId) {
        return *m_entityIdToComponent[entityId];
    }

    bool isEntityOwner(const std::string& entityId) {
        return m_entityIdToComponent.contains(entityId);
    }

private:
    std::vector<T> m_components; 
    std::unordered_map<std::string, T*> m_entityIdToComponent;
};
}
