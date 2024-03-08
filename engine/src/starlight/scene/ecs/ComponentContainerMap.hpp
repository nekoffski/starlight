#pragma once

#include <unordered_map>
#include <typeindex>

#include "starlight/core/memory/Memory.hpp"

#include "starlight/scene/components/Component.h"
#include "ComponentContainer.hpp"

namespace sl {

class ComponentContainerMap {
    using Storage =
      std::unordered_map<std::type_index, UniqPtr<ComponentContainerBase>>;

public:
    template <typename T> bool hasComponent(u64 entityId) {
        return m_componentContainers[typeid(T)]->hasComponent(entityId);
    }

    template <typename Callback>
    requires Callable<Callback, void, const Component*>
    void forEachEntityComponent(u64 entityId, Callback&& callback) const {
        for (const auto& map : m_componentContainers | std::views::values)
            if (map->hasComponent(entityId)) callback(map->getComponent(entityId));
    }

    template <typename T, typename... Args>
    T* addComponent(u64 entityId, Args&&... args) {
        return getComponents<T>()->addComponent(
          entityId, std::forward<Args>(args)...
        );
    }

    template <typename T> T* getComponent(u64 entityId) {
        return getComponents<T>()->getComponent(entityId);
    }

    template <typename T> void registerContainer() {
        m_componentContainers[typeid(T)] = createUniqPtr<ComponentContainer<T>>();
    }

    template <typename T> ComponentContainer<T>* getComponents() {
        return static_cast<ComponentContainer<T>*>(
          m_componentContainers[typeid(T)].get()
        );
    }

private:
    Storage m_componentContainers;
};

}  // namespace sl
