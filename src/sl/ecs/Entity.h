#pragma once

#include <string>
#include <vector>

#include "Component.h"
#include "Registry.h"
#include "sl/core/GameObject.h"

namespace sl::ecs {

class Entity : public core::GameObject {
    friend class Registry;

public:
    explicit Entity(const std::string& name, Registry& registry);

    std::string getName() const;

    void setName(const std::string& name) {
        m_name = name;
    }

    std::string asString() const {
        return fmt::format("[Entity name: {}, id: {}]", m_name, m_id);
    }

    std::vector<std::type_index> getComponentsIndexes();

    template <typename T, typename... Args>
    T& addComponent(Args&&... args) {
        m_componentsIndexes.emplace_back(core::typeIndex<T>());
        m_registry.addComponent<T>(m_id, std::forward<Args>(args)...);

        return getComponent<T>();
    }

    template <typename T>
    void removeComponent() {
        removeComponent(core::typeIndex<T>());
    }

    void removeComponent(std::type_index index);

    template <typename T>
    T& getComponent() {
        return m_registry.getComponent<T>(m_id);
    }

    Component& getComponent(std::type_index index);

    template <typename T>
    bool hasComponent() {
        return m_registry.hasComponent<T>(m_id);
    }

    bool isActive;

private:
    std::string m_id;
    std::string m_name;
    Registry& m_registry;
    std::vector<std::type_index> m_componentsIndexes;
};
}
