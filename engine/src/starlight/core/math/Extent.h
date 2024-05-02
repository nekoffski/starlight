#pragma once

#include <concepts>

#include "starlight/core/math/Glm.h"

namespace sl {

template <typename T>
concept ExtentType = requires(T lhs, T rhs) {
    { (lhs + rhs) / 2.0f } -> std::same_as<T>;
};

template <ExtentType T> struct Extent {
    explicit Extent() : min(0.0f), max(0.0f) {}
    explicit Extent(const T& min, const T& max) :
        min(min), max(max), center((min - max) / 2.0f) {}

    T min;
    T max;
    T center;
};

struct Extent2 : Extent<math::vec2> {
    using Extent<math::vec2>::Extent;
};

struct Extent3 : Extent<math::vec3> {
    using Extent<math::vec3>::Extent;

    Extent3(const Extent2& oth) :
        Extent(
          math::vec3{ oth.min.x, oth.min.y, 0.0f },
          math::vec3{ oth.max.x, oth.max.y, 0.0f }
        ) {}

    operator Extent2() {
        return Extent2{
            math::vec2{min.x,  min.y},
            math::vec2{ max.x, max.y}
        };
    }
};

}  // namespace sl
