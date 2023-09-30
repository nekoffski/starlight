#pragma once

#include <limits>
#include <cstdint>
#include <concepts>

namespace sl {

template <typename T>
requires std::is_integral_v<T>
class Id {
    static constexpr T invalidId = std::numeric_limits<T>::max();

public:
    using Type = T;

    Id() : m_value(invalidId) {}
    Id(T value) : m_value(value) {}

    T operator*() const { return get(); }
    T get() const { return m_value; }

    Id& operator=(T value) {
        m_value = value;
        return *this;
    }

    void invalidate() { m_value = invalidId; }

    bool hasValue() const { return m_value != invalidId; }
    operator bool() const { return hasValue(); }

private:
    T m_value;
};

using Id8  = Id<uint8_t>;
using Id16 = Id<uint16_t>;
using Id32 = Id<uint32_t>;
using Id64 = Id<uint64_t>;

}  // namespace sl
