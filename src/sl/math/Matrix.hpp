#pragma once

#include <sstream>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sl::math {

using Mat4 = glm::mat4;
using Mat3 = glm::mat3;
using Quaternion = glm::quat;

template <class T, glm::qualifier Q>
glm::mat<4, 4, T, Q> lookAt(glm::vec<3, T, Q> const& eye, glm::vec<3, T, Q> const& center,
                            glm::vec<3, T, Q> const& up) {
    return glm::lookAt(eye, center, up);
}

const Mat4 identityMatrix = Mat4{1.0f};

}  // namespace sl::math

inline std::ostream& operator<<(std::ostream& ss, const sl::math::Mat4& m) {
    ss << m[0].x << " " << m[0].y << " " << m[0].z << " " << m[0].w << '\n'
       << m[1].x << " " << m[1].y << " " << m[1].z << " " << m[1].w << '\n'
       << m[2].x << " " << m[2].y << " " << m[2].z << " " << m[2].w << '\n'
       << m[3].x << " " << m[3].y << " " << m[3].z << " " << m[3].w << '\n';
    return ss;
}

inline std::ostream& operator<<(std::ostream& ss, const sl::math::Mat3& m) {
    ss << m[0].x << " " << m[0].y << " " << m[0].z << '\n'
       << m[1].x << " " << m[1].y << " " << m[1].z << '\n'
       << m[2].x << " " << m[2].y << " " << m[2].z << '\n';
    return ss;
}

namespace sl::math {

inline std::string toString(const sl::math::Mat4& m) {
    std::ostringstream ss;
    ss << m;
    return ss.str();
}

}  // namespace sl::math