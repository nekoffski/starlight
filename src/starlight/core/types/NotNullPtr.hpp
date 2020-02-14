#pragma once

#include <stdexcept>

namespace starl::core::types {

template<typename T> class NotNullPtr {
public:
    NotNullPtr(T* ptr) : m_ptr(ptr) {
        if (ptr == nullptr) {
            throw std::logic_error("Pointer could not be empty");
        }
    }

    T* operator->() {
        return m_ptr;
    }

    T operator*() {
        return *m_ptr;
    }

    NotNullPtr(const NotNullPtr&) = delete;
    NotNullPtr(NotNullPtr&&) = default;

    NotNullPtr& operator=(const NotNullPtr&) = delete;
    NotNullPtr& operator=(NotNullPtr&&) = default;

private:
    T* m_ptr;
};

}