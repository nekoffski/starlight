#pragma once

#include <utility>
#include <array>

namespace nova::core {

template <typename T> class LocalPtr {
   public:
    explicit LocalPtr() : m_pointer(nullptr) { clear(); }

    ~LocalPtr() { clear(); }

    template <typename... Args> explicit LocalPtr(Args&&... args) : m_pointer(nullptr) {
        emplace(std::forward<Args>(args)...);
    }

    template <typename... Args> void emplace(Args&&... args) {
        clear();
        m_pointer = new ((T*)m_buffer.data()) T(std::forward<Args>(args)...);
    }

    LocalPtr(const LocalPtr&)            = delete;
    LocalPtr& operator=(const LocalPtr&) = delete;

    LocalPtr(LocalPtr&& rhs)
        : m_buffer(std::exchange(rhs.m_buffer, std::array<char, sizeof(T)>{}))
        , m_pointer((T*)m_buffer.data()) {
        rhs.m_pointer = nullptr;
    }

    LocalPtr& operator=(LocalPtr&& rhs) {
        using std::swap;

        swap(rhs.m_buffer, m_buffer);
        m_pointer = (T*)m_buffer.data();

        return *this;
    }

    void clear() {
        if (m_pointer) m_pointer->T::~T();

        m_buffer.fill(0);
        m_pointer = nullptr;
    }

    T* operator->() { return m_pointer; }

   private:
    std::array<char, sizeof(T)> m_buffer;
    T* m_pointer;
};

}  // namespace nova::core
