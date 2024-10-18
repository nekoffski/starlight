#pragma once

#include <typeindex>
#include <unordered_map>

#include "starlight/core/memory/Memory.hh"

#include "Component.hh"
#include "ComponentContainer.hh"

namespace sl {

class ComponentManager {
    using ComponentContainers =
      std::unordered_map<std::type_index, OwningPtr<ComponentContainerBase>>;

public:
    template <typename T, typename... Args>
    Component<T> add(u64 entityId, Args&&... args) {
        return getComponentContainer<T>().add(entityId, std::forward<Args>(args)...);
    }

private:
    template <typename T> ComponentContainer<T>& getComponentContainer() {
        return *static_cast<ComponentContainer<T>*>(
          m_componentContainers[typeid(T)].get()
        );
    }

    ComponentContainers m_componentContainers;
};

}  // namespace sl
