#pragma once

#include <concepts>

#include "Glm.h"

namespace sl {

template <typename T>
concept ExtentType = requires(T lhs, T rhs) {
    { (lhs + rhs) / 2.0f } -> std::same_as<T>;
};

template <ExtentType T> struct Extent {
    explicit Extent(const T& min, const T& max) :
        min(min), max(max), center((min - max) / 2.0f) {}

    T min;
    T max;
    T center;
};

struct Extent2 : Extent<Vec2f> {
    using Extent::Extent;
};
struct Extent3 : Extent<Vec3f> {
    using Extent::Extent;
};

}  // namespace sl
