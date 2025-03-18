#pragma once

#include <typeindex>
#include <unordered_map>

#include "starlight/core/Core.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/memory/Memory.hh"
#include "starlight/core/containers/FlatMap.hh"

#include "Component.hh"

namespace sl {

struct ComponentContainerBase : public NonCopyable {
    virtual ~ComponentContainerBase()  = default;
    virtual void* getRaw(u64 entityId) = 0;
};

template <typename T> class ComponentContainer : public ComponentContainerBase {
    using ComponentBuffer              = FlatMap<u64, T>;
    static constexpr u64 maxComponents = 1024;

public:
    explicit ComponentContainer() : m_components(maxComponents) {}
    ComponentBuffer* operator->() { return &m_components; }

    void* getRaw(u64 entityId) override {
        return static_cast<void*>(m_components.get(entityId));
    }

private:
    ComponentBuffer m_components;
};

class ComponentManager {
    using ComponentContainers =
      std::unordered_map<std::type_index, UniquePtr<ComponentContainerBase>>;

public:
    ComponentContainerBase& getContainer(const std::type_index& index) {
        return *m_componentContainers.at(index);
    }

    template <typename T> ComponentContainer<T>& getContainer() {
        // we could calculate hash once but iterator version is very long and hard to
        // read, in case of under-performance - rewrite
        auto& type = typeid(T);

        if (not m_componentContainers.contains(type)) [[unlikely]] {
            m_componentContainers[type] = UniquePtr<ComponentContainer<T>>::create();
        }
        return *static_cast<ComponentContainer<T>*>(
          m_componentContainers.at(type).get()
        );
    }

    void clear();

public:
    ComponentContainers m_componentContainers;
};

}  // namespace sl
