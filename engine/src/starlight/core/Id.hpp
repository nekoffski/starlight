#pragma once

#include <limits>
#include <cstdint>
#include <concepts>

namespace sl {

template <typename T>
requires std::is_integral_v<T>
class _Id {
    static constexpr T invalidId = std::numeric_limits<T>::max();

public:
    using Type = T;

    _Id() : m_value(invalidId) {}
    _Id(T value) : m_value(value) {}

    T operator*() const { return get(); }
    T get() const { return m_value; }

    _Id& operator=(T value) {
        m_value = value;
        return *this;
    }

    void invalidate() { m_value = invalidId; }

    bool hasValue() const { return m_value != invalidId; }
    operator bool() const { return hasValue(); }

private:
    T m_value;
};

using Id8  = _Id<uint8_t>;
using Id16 = _Id<uint16_t>;
using Id32 = _Id<uint32_t>;
using Id64 = _Id<uint64_t>;

}  // namespace sl
