#pragma once

#include "ComponentContainer.hpp"

namespace sl::ecs {

template <typename T>
class ComponentView {
public:
    explicit ComponentView(std::shared_ptr<ComponentContainer<T>> componentContainer)
        : m_componentContainer(componentContainer)
        , m_components(componentContainer->getAll()) {
    }

    auto begin() {
        return m_components.begin();
    }

    auto end() {
        return m_components.end();
    }

    auto size() const {
        return m_components.size();
    }

    T& operator[](int index) {
        return m_components[index];
    }

    T& getByEntityId(const std::string& entityId) {
        return m_componentContainer->getByEntityId(entityId);
    }

    bool doesEntityOwnComponent(const std::string& entityId) {
        return m_componentContainer->doesEntityOwnComponent(entityId) &&
            m_componentContainer->getByEntityId(entityId).isActive;
    }

private:
    std::vector<T>& m_components;
    std::shared_ptr<ComponentContainer<T>> m_componentContainer;
};
}
