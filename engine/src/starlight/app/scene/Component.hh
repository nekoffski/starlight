#pragma once

#include <kstd/Id.hh>

#include "starlight/core/Core.hh"
#include "fwd.hh"

namespace sl {

class Component {
public:
    explicit Component(Entity& entity);

    virtual ~Component() = default;
    u64 getEntityId() const;

    virtual void onInit() {}
    virtual void onDestroy() {}

    // todo: const
    Entity& getEntity();

private:
    Entity& m_entity;
};

namespace details {

template <typename T>
class ComponentImpl
    : public kstd::Identificable<ComponentImpl<T>>,
      public Component {
public:
    template <typename... Args>
    explicit ComponentImpl(Entity& entity, Args&&... args)
        : Component(entity)
        , m_data(std::forward<Args>(args)...) {}

protected:
    T m_data;
};

}  // namespace details

template <typename T> struct ComponentBase : details::ComponentImpl<T> {
    using details::ComponentImpl<T>::ComponentImpl;

    T& data() { return this->m_data; }
    const T& data() const { return this->m_data; }
    T* operator->() { return &this->m_data; }
};

template <typename T>
struct ComponentBase<kstd::SharedPtr<T>>
    : details::ComponentImpl<kstd::SharedPtr<T>> {
    using details::ComponentImpl<kstd::SharedPtr<T>>::ComponentImpl;

    T& data() { return *this->m_data; }
    const T& data() const { return *this->m_data; }
    T* operator->() { return this->m_data.get(); }
};

}  // namespace sl
