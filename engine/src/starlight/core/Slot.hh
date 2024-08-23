#pragma once

#include <kc/core/Countable.hpp>

namespace sl {

template <typename T> class Slot : public kc::core::Countable<Slot<T>> {
public:
    Slot() : m_value(nullptr), m_taken(false), m_id(getId()) {}
    explicit Slot(T* value) : m_value(value), m_taken(true), m_id(getId()) {}

    operator bool() const { return m_taken; }

    void set(T* value) {
        m_value = value;
        m_taken = true;
    }

    T* get() { return m_value; }

    void reset() {
        m_taken = false;
        m_value = nullptr;
    }

private:
    T* m_value;
    bool m_taken;
    u32 m_id;
};

}  // namespace sl
