#pragma once

#include <string>
#include <vector>

#include "Component.h"
#include "Registry.h"
#include "sl/core/misc/misc.hpp"
#include "sl/gui/GuiApi.h"

namespace sl::ecs {

class Entity {
    friend class Registry;

public:
    explicit Entity(const std::string& id, const std::string& name, Registry& registry)
        : m_id(id)
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
        m_componentsIndexes.emplace_back(core::misc::typeIndex<T>());
        m_registry.addComponent<T>(m_id, std::forward<Args>(args)...);
    }

    std::vector<std::type_index> getComponentsIndexes() {
        return m_componentsIndexes;
    }

    template <typename T>
    Component& getComponent() {
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
        for (const auto& index : m_componentsIndexes)
            m_registry.getComponentByIndex(m_id, index).onGui(gui);
    }

private:
    std::string m_id;
    std::string m_name;
    Registry& m_registry;
    std::vector<std::type_index> m_componentsIndexes;
};
}
