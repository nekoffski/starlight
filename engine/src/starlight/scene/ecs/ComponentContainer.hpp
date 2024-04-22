#pragma once

#include <unordered_map>

#include "starlight/core/utils/ResourcePool.hpp"
#include "starlight/core/Core.h"

#include "starlight/scene/components/Component.h"

namespace sl {

static constexpr u64 defaultMaxComponents = 512;

struct ComponentContainerBase {
    virtual ~ComponentContainerBase() = default;

    virtual void clear()                          = 0;
    virtual bool hasComponent(u64 entityId)       = 0;
    virtual Component* getComponent(u64 entityId) = 0;
};

template <typename T> class ComponentContainer : public ComponentContainerBase {
public:
    explicit ComponentContainer() :
        m_buffer(typeid(T).name(), defaultMaxComponents) {}

    template <typename... Args> T* addComponent(u64 entityId, Args&&... args) {
        auto component   = m_buffer.create(entityId, std::forward<Args>(args)...);
        m_view[entityId] = component;
        return component;
    }

    T* getComponent(u64 entityId) override {
        return m_view[entityId];  // TODO: error checking
    }

    template <typename Callback>
    requires Callable<Callback, void, T*>
    void forEach(Callback&& callback) {
        for (auto& component : m_view | std::views::values) callback(component);
    }

    bool hasComponent(u64 entityId) override { return m_view.contains(entityId); }

    void clear() override {
        m_buffer.clear();
        m_view.clear();
    }

private:
    ResourcePool<T> m_buffer;
    std::unordered_map<u64, T*> m_view;  // TODO: flatmap
};

}  // namespace sl
