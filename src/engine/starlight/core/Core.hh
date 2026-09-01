#pragma once

#include <fmt/core.h>

#include <cstdint>
#include <limits>
#include <numeric>
#include <optional>
#include <span>
#include <vector>

namespace sl {

template <typename T>
T maxOf() {
    return std::numeric_limits<T>::max();
}

template <typename T>
T minOf() {
    return std::numeric_limits<T>::min();
}

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

template <typename T>
using Opt = std::optional<T>;

using Nanoseconds = u64;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8 = int8_t;

using f32 = float;
using f64 = double;
using Str = std::string;

using Byte = u8;
using Bytes = std::vector<Byte>;

using BytesView = std::span<Byte>;
using CBytesView = std::span<const Byte>;

using Id = u16;

template <typename T>
T max() {
    return std::numeric_limits<T>::max();
}

template <typename T>
T min() {
    return std::numeric_limits<T>::min();
}

template <typename T>
class Wrapper {
   public:
    explicit Wrapper(const T& value) : m_value(value) {}

    const T& get() const { return m_value; }

    bool operator==(const Wrapper& other) const {
        return m_value == other.m_value;
    }

    bool operator!=(const Wrapper& other) const {
        return m_value != other.m_value;
    }

   private:
    T m_value;
};

using Name = Wrapper<Str>;

template <typename T>
class Tag : public Wrapper<T> {
   public:
    using Wrapper<T>::Wrapper;
};

template <>
class Tag<Str> : public Wrapper<Str> {
   public:
    using Wrapper<Str>::Wrapper;

    static Tag<Str> fromUuid();
};

}  // namespace sl

template <typename T>
struct std::hash<sl::Wrapper<T>> {
    std::size_t operator()(const sl::Wrapper<T>& v) const noexcept {
        return std::hash<T>{}(v.get());
    }
};

template <typename T>
struct std::hash<sl::Tag<T>> {
    std::size_t operator()(const sl::Tag<T>& v) const noexcept {
        return std::hash<T>{}(v.get());
    }
};
