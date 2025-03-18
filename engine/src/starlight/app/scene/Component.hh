#pragma once

#include "starlight/core/Id.hh"
#include "starlight/core/memory/Memory.hh"

#include "fwd.hh"

namespace sl {

class ComponentBase {
public:
    explicit ComponentBase(Entity& entity);

    virtual ~ComponentBase() = default;
    u64 getEntityId() const;

private:
    Entity& m_entity;
};

namespace detail {

template <typename T>
class ComponentImpl : public Identificable<ComponentImpl<T>>, public ComponentBase {
public:
    template <typename... Args>
    explicit ComponentImpl(Entity& entity, Args&&... args) :
        ComponentBase(entity), m_data(std::forward<Args>(args)...) {}

protected:
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
