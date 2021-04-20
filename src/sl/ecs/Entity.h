#pragma once

#include <string>
#include <vector>

#include "Component.h"
#include "Registry.h"
#include "sl/core/Misc.hpp"
#include "sl/gui/GuiApi.h"

namespace sl::ecs {

class Entity {
    friend class Registry;

public:
    explicit Entity(const std::string& id, const std::string& name, Registry& registry)
        : isActive(true)
        , m_id(id)
        , m_name(name)
        , m_registry(registry) {
    }

    std::string getId() const {
        return m_id;
    }

    std::string getName() const {
        return m_name;
    }

    template <typename T, typename... Args>
    void addComponent(Args&&... args) {
        m_componentsIndexes.emplace_back(core::typeIndex<T>());
        m_registry.addComponent<T>(m_id, std::forward<Args>(args)...);
    }

    template <typename T>
    void removeComponent() {
        std::erase(m_componentsIndexes, core::typeIndex<T>());
        m_registry.removeComponent(m_id, core::typeIndex<T>());
    }

    std::vector<std::type_index> getComponentsIndexes() {
        return m_componentsIndexes;
    }

    template <typename T>
    T& getComponent() {
        return m_registry.getComponent<T>(m_id);
    }

    Component& getComponent(std::type_index index) {
        return m_registry.getComponentByIndex(m_id, index);
    }

    template <typename T>
    bool hasComponent() {
        return m_registry.hasComponent<T>(m_id);
    }

    void onGui(sl::gui::GuiApi& gui) {
        std::vector<std::type_index> indexesToRemove;

        for (const auto& index : m_componentsIndexes) {
            auto& component = m_registry.getComponentByIndex(m_id, index);
            component.onGui(gui);

            if (component.shouldBeRemoved)
                indexesToRemove.push_back(index);
        }

        for (const auto& index : indexesToRemove) {
            m_registry.removeComponent(m_id, index);
            std::erase(m_componentsIndexes, index);
        }
    }

    bool isActive;

private:
    std::string m_id;
    std::string m_name;
    Registry& m_registry;
    std::vector<std::type_index> m_componentsIndexes;
};
}
