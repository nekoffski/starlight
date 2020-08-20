#pragma once

#include <string>
#include <vector>

#include "Registry.h"
#include "starlight/gui/Window.h"

namespace sl::ecs {

class Entity {
    friend class Registry;

public:
    explicit Entity(std::string id, std::string name, Registry& registry)
        : m_id(std::move(id))
        , m_name(std::move(name))
        , m_registry(registry) {
    }

    std::string getId() const {
        return m_id;
    }

    std::string getName() const {
        return m_name;
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

    void onGUI(gui::Window& window) {
        for (const auto& index : m_componentsIndexes)
            m_registry.wrapComponentByIndex(index, m_id)->onGUI(window);
    }

private:
    std::string m_id;
    std::string m_name;
    Registry& m_registry;
    std::vector<std::type_index> m_componentsIndexes;
};
}