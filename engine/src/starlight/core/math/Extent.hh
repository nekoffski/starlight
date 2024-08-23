#pragma once

#include <concepts>

#include "starlight/core/math/Glm.hh"
#include "starlight/core/math/Vec.hh"

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

struct Extent2 : Extent<Vec2<f32>> {
    using Extent<Vec2<f32>>::Extent;
};

struct Extent3 : Extent<Vec3<f32>> {
    using Extent<Vec3<f32>>::Extent;

    Extent3(const Extent2& oth) :
        Extent(
          Vec3<f32>{ oth.min.x, oth.min.y, 0.0f },
          Vec3<f32>{ oth.max.x, oth.max.y, 0.0f }
        ) {}

    operator Extent2() {
        return Extent2{
            Vec2<f32>{min.x,  min.y},
            Vec2<f32>{ max.x, max.y}
        };
    }
};

}  // namespace sl
