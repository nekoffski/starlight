#pragma once

#include <stdexcept>

namespace sl::misc::types {

template <typename T>
class NotNullPtrBase {
public:
    NotNullPtrBase(T* ptr)
        : m_ptr(ptr) {
        if (ptr == nullptr)
            throw std::logic_error("Pointer could not be empty");
    }

    operator T*() const { return m_ptr; }

    T* operator->() { return m_ptr; }

    NotNullPtrBase(const NotNullPtrBase&) = default;
    NotNullPtrBase(NotNullPtrBase&&) = default;

    NotNullPtrBase& operator=(const NotNullPtrBase&) = default;
    NotNullPtrBase& operator=(NotNullPtrBase&&) = default;

protected:
    T* m_ptr;
};

template <typename T>
class NotNullPtr : public NotNullPtrBase<T> {
public:
    using NotNullPtrBase<T>::NotNullPtrBase;

    T& operator*() { return *this->m_ptr; }
};

template <>
class NotNullPtr<void> : public NotNullPtrBase<void> {
public:
    using NotNullPtrBase<void>::NotNullPtrBase;
};
}
