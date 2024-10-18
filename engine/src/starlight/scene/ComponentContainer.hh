#pragma once

#include <vector>

#include "starlight/core/Core.hh"

#include "Component.hh"

namespace sl {

struct ComponentContainerBase {
    virtual ~ComponentContainerBase() = default;
};

template <typename T> class ComponentContainer : public ComponentContainerBase {
    using ComponentBuffer = std::vector<Component<T>>;

public:
    explicit ComponentContainer() {
        // TODO: replace with StableVector or FlatMap
        // actually, seems like we can use StableVector as a map as entities id is
        // just a sequence of numbers
        m_components.reserve(1000);
    }

    template <typename... Args> Component<T>& add(u64 entityId, Args&&... args) {
        m_components.emplace_back(entityId, std::forward<Args>(args)...);
        return m_components.back();
    }

    template <typename C>
    requires Callable<C, void, Component<T>&>
    void forEach(C&& callback) {
        for (auto& component : m_components) callback(component);
    }

private:
    ComponentBuffer m_components;
};

}  // namespace sl
