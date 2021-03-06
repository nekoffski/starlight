#pragma once

#include <cmath>
#include <numbers>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Matrix.hpp"
#include "Vector.hpp"

#include <glm/gtx/matrix_decompose.hpp>

// TODO: HIDE IMPLEMENTATION OF GLM

namespace sl::math {

// TODO: this one seems broken
template <typename T>
inline const T circularRange(const T& value, const T min, const T max) {
    if (value < min)
        return max;
    else if (value > max)
        return min;
    return value;
}

template <class T, glm::qualifier Q>
inline glm::mat<4, 4, T, Q> quaternionToMat4(glm::tquat<T, Q> const& x) {
    return mat4_cast(x);
}

template <class T, glm::qualifier Q>
inline T const* valuePtr(glm::mat<3, 3, T, Q> const& m) {
    return &(m[0].x);
}

template <class T, glm::qualifier Q>
inline T* valuePtr(glm::mat<3, 3, T, Q>& m) {
    return &(m[0].x);
}

template <class T, glm::qualifier Q>
inline T const* valuePtr(glm::mat<4, 4, T, Q> const& m) {
    return &(m[0].x);
}

template <class T, glm::qualifier Q>
inline T* valuePtr(glm::mat<4, 4, T, Q>& m) {
    return &(m[0].x);
}

template <typename T>
inline glm::mat<4, 4, T, glm::defaultp> perspective(T fovy, T aspect, T zNear, T zFar) {
    return glm::perspective(fovy, aspect, zNear, zFar);
}

inline float toDegrees(float rad) {
    return glm::degrees(rad);
}

inline Vec3 toDegrees(const Vec3& vec) {
    return Vec3 { toDegrees(vec.x), toDegrees(vec.y), toDegrees(vec.z) };
}

inline Mat4 translate(Mat4& mat, Vec3 transition) {
    return glm::translate(mat, std::move(transition));
}

inline Mat4 translate(Vec3 transition) {
    return translate(math::Mat4(1.0f), std::move(transition));
}

inline Mat4 scale(Vec3 scale) {
    return glm::scale(glm::mat4(1.0f), scale);
}

inline Mat4 scale(float s) {
    return scale(Vec3 { s });
}

inline float toRadians(float deg) {
    return glm::radians(deg);
}

inline Vec3 toRadians(const Vec3& vec) {
    return Vec3 { toRadians(vec.x), toRadians(vec.y), toRadians(vec.z) };
}

inline Vec3 normalize(const Vec3& vec) {
    return glm::normalize(vec);
}

inline Mat4 createRotationMatrix(const math::Vec3& orientation) {
    return quaternionToMat4(Quaternion(orientation));
}

inline float distance(const math::Vec3& lhs, const math::Vec3& rhs) {
    return glm::distance(lhs, rhs);
}

template <typename T>
inline float lerp(const T& a, const T& b, float alfa) {
    return std::lerp(a, b, alfa);
}

inline math::Vec3 lerp(const math::Vec3& a, const math::Vec3& b, float alfa) {
    return (1.0f - alfa) * a + alfa * b;
}

template <typename T>
inline T ReLU(const T& lhs) {
    return std::max(0, lhs);
}

inline void decomposeMatrix(const Mat4& transform, Vec3& translation, Vec3& rotation, Vec3& scale) {
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::quat rotationQuat;

    glm::decompose(transform, scale, rotationQuat, translation, skew, perspective);
    rotation = glm::eulerAngles(rotationQuat);
}
}

inline sl::math::Vec3 operator*(const sl::math::Mat4& lhs, const sl::math::Vec3& rhs) {
    return sl::math::Vec3 { lhs * sl::math::Vec4 { rhs, 1.0f } };
}