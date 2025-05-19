#pragma once

#include <tuple>
#include <optional>
#include <concepts>
#include <cmath>

#include "Core.hh"
#include "Glm.hh"
#include "Vec.hh"

namespace sl {

template <typename T>
requires std::is_floating_point_v<T>
std::optional<std::pair<T, T>> solveQuadraticEquation(T a, T b, T c) {
    T delta = b * b - 4 * a * c;
    if (delta < 0) {
        return {};
    } else if (delta == 0) {
        T x = -0.5 * b / a;
        return std::make_pair(x, x);
    }

    T deltaSqrt = std::sqrt(delta);
    T q         = (b > 0) ? -0.5 * (b + deltaSqrt) : -0.5 * (b - deltaSqrt);

    T x0 = q / a;
    T x1 = c / q;

    if (x0 > x1) std::swap(x0, x1);

    return std::make_pair(x0, x1);
}

Vec3<f32> deproject(
  const Vec2<f32>& mouseCords, const Vec2<u32>& viewport,
  const Mat4<f32>& invProjectionMatrix, const Mat4<f32>& invViewMatrix
);

struct MatrixComponents {
    Vec3<f32> scale;
    Quat rotation;
    Vec3<f32> translation;
    Vec3<f32> euler;
    Vec3<f32> skew;
    Vec4<f32> perspective;
};

MatrixComponents decomposeTransformation(const Mat4<f32>& matrix);

}  // namespace sl
