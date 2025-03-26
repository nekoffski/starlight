#pragma once

#include <numbers>

#include "starlight/core/Core.hh"
#include "starlight/core/Concepts.hh"

#include "Glm.hh"
#include "Vec.hh"
#include "Rect.hh"
#include "Extent.hh"

namespace sl {

const Mat4<f32> identityMatrix = Mat4<f32>{ 1.0f };
const float pi                 = std::numbers::pi_v<float>;

template <typename T> constexpr u64 getSize() { return 0u; }

using Quat = math::quat;

template <> constexpr u64 getSize<Vec2<f32>>() { return 2u; }
template <> constexpr u64 getSize<Vec3<f32>>() { return 3u; }
template <> constexpr u64 getSize<Vec4<f32>>() { return 4u; }

template <> constexpr u64 getSize<Mat2<f32>>() { return 4u; }
template <> constexpr u64 getSize<Mat3<f32>>() { return 9u; }
template <> constexpr u64 getSize<Mat4<f32>>() { return 16u; }

template <typename T, typename F = f32>
requires OneOf<T, Vec2<f32>, Vec3<f32>, Vec4<f32>>
bool almostEquals(const T& lhs, const T& rhs, F delta) {
    for (u64 i = 0; i < getSize<T>(); ++i)
        if (std::fabs(rhs[i] - lhs[i]) > delta) return false;
    return true;
}

template <typename T>
requires OneOf<T, Vec2<f32>, Vec3<f32>, Vec4<f32>>
bool almostNull(const T& lhs) {
    static constexpr f32 delta = 0.001f;
    return math::length2(lhs) <= delta;
}

}  // namespace sl
