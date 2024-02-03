#pragma once

#include <unordered_map>
#include <typeindex>

#include "starlight/core/memory/Memory.hpp"

#include "ComponentContainer.hpp"

namespace sl {

class ComponentContainerMap {
    using Storage =
      std::unordered_map<std::type_index, UniqPtr<ComponentContainerBase>>;

public:
    template <typename T> bool hasComponent(u64 id) {
        return m_componentContainers[typeid(T)]->hasComponent(id);
    }

    template <typename T, typename... Args> T* addComponent(u64 id, Args&&... args) {
        return getContainer<T>()->addComponent(id, std::forward<Args>(args)...);
    }

    template <typename T> T* getComponent(u64 id) {
        return getContainer<T>()->getComponent(id);
    }

    template <typename T> void registerContainer() {
        m_componentContainers[typeid(T)] = createUniqPtr<ComponentContainer<T>>();
    }

private:
    template <typename T> ComponentContainer<T>* getContainer() {
        return static_cast<ComponentContainer<T>*>(
          m_componentContainers[typeid(T)].get()
        );
    }

    Storage m_componentContainers;
};

}  // namespace sl
