#pragma once

#include <cmath>

#include <glm/gtc/type_ptr.hpp>

namespace starl::math {

const double PI = std::atan(1.0f) * 4.0f;

template <typename T, glm::qualifier Q>
T const* valuePtr(glm::mat<3, 3, T, Q> const& m) {
    return &(m[0].x);
}

template <typename T, glm::qualifier Q>
T* valuePtr(glm::mat<3, 3, T, Q>& m) {
    return &(m[0].x);
}

template <typename T, glm::qualifier Q>
T const* valuePtr(glm::mat<4, 4, T, Q> const& m) {
    return &(m[0].x);
}

template <typename T, glm::qualifier Q>
T* valuePtr(glm::mat<4, 4, T, Q>& m) {
    return &(m[0].x);
}

template <typename T>
glm::mat<4, 4, T, glm::defaultp> perspective(T fovy, T aspect, T zNear, T zFar) {
    return glm::perspective(fovy, aspect, zNear, zFar);
}
}