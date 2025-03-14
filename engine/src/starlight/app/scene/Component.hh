#pragma once

#include "starlight/core/Id.hh"
#include "starlight/core/memory/Memory.hh"

namespace sl {

struct ComponentBase {
    virtual ~ComponentBase()        = default;
    virtual u64 getEntityId() const = 0;
};

namespace detail {

template <typename T>
class ComponentImpl : public Identificable<ComponentImpl<T>>, public ComponentBase {
public:
    template <typename... Args>
    explicit ComponentImpl(u64 entityId, Args&&... args) :
        m_entityId(entityId), m_data(std::forward<Args>(args)...) {}

    u64 getEntityId() const override { return m_entityId; }

protected:
    u64 m_entityId;
    T m_data;
};

}  // namespace detail

template <typename T> struct Component : detail::ComponentImpl<T> {
    using detail::ComponentImpl<T>::ComponentImpl;

    T& data() { return this->m_data; }
    const T& data() const { this->m_data; }
    T* operator->() { return &this->m_data; }
};

template <typename T>
struct Component<SharedPtr<T>> : detail::ComponentImpl<SharedPtr<T>> {
    using detail::ComponentImpl<SharedPtr<T>>::ComponentImpl;

    T& data() { return *this->m_data; }
    const T& data() const { return *this->m_data; }
    T* operator->() { return this->m_data.get(); }
};

}  // namespace sl
