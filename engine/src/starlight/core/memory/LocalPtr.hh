#pragma once

#include <utility>
#include <array>
#include <type_traits>
#include "starlight/core/Core.hh"

namespace sl {

template <typename T> class LocalPtr : public NonCopyable {
public:
    explicit LocalPtr() : m_pointer(nullptr) { clear(); }

    ~LocalPtr() { clear(); }

    template <typename... Args>
    explicit LocalPtr(Args&&... args) : m_pointer(nullptr) {
        emplace(std::forward<Args>(args)...);
    }

    template <typename... Args> T* emplace(Args&&... args) {
        clear();
        m_pointer = new ((T*)&m_buffer) T(std::forward<Args>(args)...);
        return m_pointer;
    }

    T* get() { return m_pointer; }
    const T* get() const { return m_pointer; }

    T& operator*() { return *m_pointer; }

    operator bool() const { return m_pointer != nullptr; }

    LocalPtr(const LocalPtr&)            = delete;
    LocalPtr& operator=(const LocalPtr&) = delete;

    LocalPtr(LocalPtr&& rhs) {
        std::memcpy(m_buffer, rhs.m_buffer, sizeof(m_buffer));
        m_pointer = (T*)&m_buffer;
        rhs.clear(false);
    }

    LocalPtr& operator=(LocalPtr&& rhs) {
        clear();

        using std::swap;
        swap(rhs.m_buffer, m_buffer);
        m_pointer     = (T*)&m_buffer;
        rhs.m_pointer = nullptr;

        return *this;
    }

    void clear(bool destroy = true) {
        if (m_pointer && destroy) m_pointer->T::~T();

        std::memset(&m_buffer, 0, sizeof(m_buffer));
        m_pointer = nullptr;
    }

    T* operator->() { return m_pointer; }

private:
    alignas(T) std::byte m_buffer[sizeof(T)];

    T* m_pointer;
};

}  // namespace sl
