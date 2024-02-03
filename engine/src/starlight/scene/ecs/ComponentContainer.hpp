#pragma once

#include "starlight/core/utils/ResourcePool.hpp"
#include "starlight/core/Core.h"

#include <unordered_map>

namespace sl {

static constexpr u64 defaultMaxComponents = 512;

struct ComponentContainerBase {
    virtual bool hasComponent(u64 id) = 0;
};

template <typename T> class ComponentContainer : public ComponentContainerBase {
public:
    explicit ComponentContainer() :
        m_buffer(typeid(T).name(), defaultMaxComponents) {}

    template <typename... Args> T* addComponent(u64 id, Args&&... args) {
        auto component = m_buffer.create(std::forward<Args>(args)...);
        m_view[id]     = component;
        return component;
    }

    template <typename... Args> T* getComponent(u64 id) {
        return m_view[id];  // TODO: error checking
    }

    bool hasComponent(u64 id) override { return m_view.contains(id); }

private:
    ResourcePool<T> m_buffer;
    std::unordered_map<u64, T*> m_view;  // TODO: flatmap
};

}  // namespace sl
