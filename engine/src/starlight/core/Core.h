#pragma once

#include <cstdint>
#include <string>
#include <concepts>

#include <kc/core/ErrorBase.hpp>
#include <tl/expected.hpp>

namespace sl {

DEFINE_ERROR(SLError);

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8  = uint8_t;

struct Range {
    u64 offset;
    u64 size;

    static Range aligned(u64 offset, u64 size, u64 granularity);
};

template <typename T> struct Interval {
    T min;
    T max;
};

template <typename T> struct StepInterval {
    T min;
    T max;
    T step;
};

using Intervalf   = Interval<float>;
using Intervalu32 = Interval<u32>;
using Intervalu64 = Interval<u64>;

template <typename T> using Expected = tl::expected<T, SLError>;

std::string_view removeExtension(std::string_view path);

template <typename T> T getAlignedValue(T value, T granularity) {
    return (value + granularity - 1) & ~(granularity - 1);
}

template <typename C, typename R = void, typename... Args>
concept Callable = requires(C&& callback, Args&&... args) {
    { callback(std::forward<Args>(args)...) } -> std::same_as<R>;
};

}  // namespace sl
