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

    virtual void onInit() {}
    virtual void onDestroy() {}

    // todo: const
    Entity& getEntity();

private:
    Entity& m_entity;
};

namespace details {

template <typename T>
class ComponentImpl : public Identificable<ComponentImpl<T>>, public ComponentBase {
public:
    template <typename... Args>
    explicit ComponentImpl(Entity& entity, Args&&... args)
        : ComponentBase(entity)
        , m_data(std::forward<Args>(args)...) {}

protected:
    T m_data;
};

}  // namespace details

template <typename T> struct Component : details::ComponentImpl<T> {
    using details::ComponentImpl<T>::ComponentImpl;

    T& data() { return this->m_data; }
    const T& data() const { return this->m_data; }
    T* operator->() { return &this->m_data; }
};

template <typename T>
struct Component<SharedPtr<T>> : details::ComponentImpl<SharedPtr<T>> {
    using details::ComponentImpl<SharedPtr<T>>::ComponentImpl;

    T& data() { return *this->m_data; }
    const T& data() const { return *this->m_data; }
    T* operator->() { return this->m_data.get(); }
};

}  // namespace sl
