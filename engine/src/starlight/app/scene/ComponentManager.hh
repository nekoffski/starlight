#pragma once

#include <typeindex>
#include <unordered_map>

#include <kstd/containers/FlatMap.hh>

#include "starlight/core/Core.hh"
#include "starlight/core/Concepts.hh"

#include "Component.hh"

namespace sl {

struct ComponentContainer : public kstd::NonCopyable {
    virtual ~ComponentContainer()      = default;
    virtual void* getRaw(u64 entityId) = 0;
};

template <typename T> class ComponentContainerBase : public ComponentContainer {
    static constexpr u64 maxComponents = 1024;
    using ComponentBuffer              = kstd::StaticFlatMap<u64, T, maxComponents>;

public:
    ComponentBuffer* operator->() { return &m_components; }

    void* getRaw(u64 entityId) override {
        return static_cast<void*>(m_components.get(entityId));
    }

private:
    ComponentBuffer m_components;
};

class ComponentManager {
    using ComponentContainers =
      std::unordered_map<std::type_index, kstd::UniquePtr<ComponentContainer>>;

public:
    ComponentContainer& getContainer(const std::type_index& index) {
        return *m_componentContainers.at(index);
    }

    template <typename T> ComponentContainerBase<T>& getContainer() {
        // we could calculate hash once but iterator version is very long and hard to
        // read, in case of under-performance - rewrite
        auto& type = typeid(T);

        if (not m_componentContainers.contains(type)) [[unlikely]] {
            m_componentContainers[type] =
              kstd::makeUnique<ComponentContainerBase<T>>();
        }
        return *static_cast<ComponentContainerBase<T>*>(
          m_componentContainers.at(type).get()
        );
    }

    void clear();

public:
    ComponentContainers m_componentContainers;
};

}  // namespace sl
