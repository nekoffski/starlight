#pragma once

#include <cmath>

#include "Glm.hh"

namespace sl {

using Vec2f = glm::vec2;
using Vec3f = glm::vec3;
using Vec4f = glm::vec4;

using Vec2i = glm::ivec2;
using Vec3i = glm::ivec3;
using Vec4i = glm::ivec4;

using Vec2u = glm::uvec2;
using Vec3u = glm::uvec3;
using Vec4u = glm::uvec4;

using Mat2f = glm::mat2;
using Mat3f = glm::mat3;
using Mat4f = glm::mat4;

using Quat = glm::quat;

inline bool isFinite(const Mat4f& matrix) {
    for (int column = 0; column < matrix.length(); ++column) {
        for (int row = 0; row < matrix[column].length(); ++row) {
            if (not std::isfinite(matrix[column][row])) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace sl
