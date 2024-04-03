#pragma once

#include <functional>

namespace sl {

template <typename T> class RAIIWrapper {
    using Destructor = std::function<void(T&)>;

public:
    template <typename Constructor>
    explicit RAIIWrapper(Constructor&& constructor, Destructor&& destructor) :
        m_resource(constructor()), m_destructor(std::move(destructor)) {}

    ~RAIIWrapper() { m_destructor(m_resource); }

    T& get() const { return m_resource; }
    T* getPointer() const { return &m_resource; }

private:
    mutable T m_resource;  // todo: no mutable!
    Destructor m_destructor;
};

}  // namespace sl
