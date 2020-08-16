#pragma once

#include <string>

#include "Registry.h"

namespace sl::ecs {

class Entity {
public:
    explicit Entity(std::string id, Registry& registry)
        : m_id(std::move(id))
        , m_registry(registry) {
    }

    std::string getId() const {
        return m_id;
    }

    template <typename Component, typename... Args>
    void addComponent(Args&&... args) {
        m_registry.addComponent<Component>(m_id, std::forward<Args>(args)...);
    }

    template <typename Component>
    Component& getComponent() {
        return m_registry.getComponent<Component>(m_id);
    }

    template <typename Component>
    bool hasComponent() {
        return m_registry.hasComponent<Component>(m_id);
    }

private:
    std::string m_id;
    Registry& m_registry;
};
}