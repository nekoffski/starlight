#pragma once

#include <glm/geometric.hpp>
#include <sstream>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/glm.hpp>

namespace sl::math {

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

template <glm::length_t L, class T, glm::qualifier Q>
glm::vec<L, T, Q> normalize(glm::vec<L, T, Q> const& x) {
    return glm::normalize(x);
}

template <class T, glm::qualifier Q>
glm::vec<3, T, Q> cross(glm::vec<3, T, Q> const& x, glm::vec<3, T, Q> const& y) {
    return glm::cross(x, y);
}

template <class T, glm::qualifier Q>
float dot(glm::vec<3, T, Q> const& x, glm::vec<3, T, Q> const& y) {
    return glm::dot(x, y);
}

template <class T, glm::qualifier Q>
float distance(glm::vec<3, T, Q> const& x, glm::vec<3, T, Q> const& y) {
    return glm::distance(x, y);
}

inline Vec4 asColor(const Vec3& vec) {
    return Vec4 { vec.x / 255.0f, vec.y / 255.0f, vec.z / 255.0f, 1.0f };
}

inline Vec4 asColor(const Vec4& vec) {
    return Vec4 { vec.x / 255.0f, vec.y / 255.0f, vec.z / 255.0f, vec.w / 255.0f };
}

inline const math::Vec3 VEC_ONES = math::Vec3 { 1.0f, 1.0f, 1.0f };
inline const math::Vec3 VEC_ZEROS = math::Vec3 { 0.0f, 0.0f, 0.0f };
}

inline std::ostream& operator<<(std::ostream& ss, const sl::math::Vec4& v) {
    ss << v.x << " " << v.y << " " << v.z << " " << v.w << '\n';
    return ss;
}

inline std::ostream& operator<<(std::ostream& ss, const sl::math::Vec3& v) {
    ss << v.x << " " << v.y << " " << v.z << '\n';
    return ss;
}

inline std::ostream& operator<<(std::ostream& ss, const sl::math::Vec2& v) {
    ss << v.x << " " << v.y << '\n';
    return ss;
}
