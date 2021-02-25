#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace sl::ecs {

class IComponentContainer {
public:
    virtual ~IComponentContainer() = default;
};

template <typename T>
class ComponentContainer : public IComponentContainer {
public:
    inline static constexpr int defaultCapacity = 1000;

    static std::shared_ptr<IComponentContainer> create() {
        return std::make_shared<ComponentContainer<T>>();
    }

    explicit ComponentContainer(const int capacity = defaultCapacity) {
        m_components.reserve(capacity);
    }

    template <typename... Args>
    T& add(const std::string& entityId, Args&&... args) {
        m_components.emplace_back(T{ std::forward<Args>(args)... });
        auto& component = m_components.back();

        component.ownerEntityId = entityId;
        m_entityIdToComponent[entityId] = &component;

        return component;
    }

    void remove(const std::string& entityId) {
        m_entityIdToComponent.erase(entityId);
        std::erase_if(m_components, [&entityId](T& component) { return component.ownerEntityId == entityId; });
        rebuildMap();
    }

    std::size_t size() const {
        return m_components.size();
    }

    std::size_t capacity() const {
        return m_components.capacity();
    }

    std::vector<T>& getAll() {
        return m_components;
    }

    T& getByEntityId(const std::string& entityId) {
        return *m_entityIdToComponent[entityId];
    }

    bool doesEntityOwnComponent(const std::string& entityId) {
        return m_entityIdToComponent.contains(entityId);
    }

private:
    void rebuildMap() {
        m_entityIdToComponent.clear();
        for (auto& component : m_components)
            m_entityIdToComponent[component.ownerEntityId] = &component;
    }

    std::vector<T> m_components;
    std::unordered_map<std::string, T*> m_entityIdToComponent;
};
}
