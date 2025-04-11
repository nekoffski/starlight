#pragma once

#include <optional>
#include <vector>

#include "starlight/core/Id.hh"

#include "Component.hh"
#include "ComponentManager.hh"

namespace sl {

class Entity : public NamedResource<Entity, "Entity", false> {
public:
    explicit Entity(
      ComponentManager& componentManager, std::optional<std::string> name
    );

    template <typename T, typename... Args>
    requires std::derived_from<T, Component>
    T& add(Args&&... args) {
        auto& container = m_componentManager.getContainer<T>();
        log::expect(
          not container->has(id), "Could not add the same component twice"
        );
        m_componentTypes.emplace_back(typeid(T));
        auto component = container->emplace(id, *this, std::forward<Args>(args)...);
        component->onInit();
        return *component;
    }

    template <typename T>
    requires std::derived_from<T, Component>
    T* get() {
        return m_componentManager.getContainer<T>()->get(id);
    }

    template <typename T>
    requires std::derived_from<T, Component>
    bool has() {
        return m_componentManager.getContainer<T>()->has(id);
    }

    void* get(std::type_index component) {
        return m_componentManager.getContainer(component).getRaw(id);
    }

    std::span<const std::type_index> getComponentTypes() const {
        return m_componentTypes;
    }

private:
    ComponentManager& m_componentManager;
    std::vector<std::type_index> m_componentTypes;
};

}  // namespace sl
