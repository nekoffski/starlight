#pragma once

#include <cmath>

#include <glm/gtc/type_ptr.hpp>

namespace starl::math {

const double PI = std::atan(1.0f) * 4.0f;

template <typename T>
inline const T circularRange(const T& value, const T min, const T max) {
    if (value < min) {
        return max;
    } else if (value > max) {
        return min;
    }
    return value;
}

template <typename T, glm::qualifier Q>
inline T const* valuePtr(glm::mat<3, 3, T, Q> const& m) {
    return &(m[0].x);
}

template <typename T, glm::qualifier Q>
inline T* valuePtr(glm::mat<3, 3, T, Q>& m) {
    return &(m[0].x);
}

template <typename T, glm::qualifier Q>
inline T const* valuePtr(glm::mat<4, 4, T, Q> const& m) {
    return &(m[0].x);
}

template <typename T, glm::qualifier Q>
inline T* valuePtr(glm::mat<4, 4, T, Q>& m) {
    return &(m[0].x);
}

template <typename T>
inline glm::mat<4, 4, T, glm::defaultp> perspective(T fovy, T aspect, T zNear, T zFar) {
    return glm::perspective(fovy, aspect, zNear, zFar);
}
}