#pragma once

#include <glm/glm.hpp>

namespace sl::math {

inline glm::vec3 viewportToWorld(const glm::vec2& viewportPosition, const glm::mat4& viewMatrix,
                                 const glm::mat4& projectionMatrix, const glm::vec2& windowSize) {
    // clang-format off
    glm::vec3 rayNdc {
        (2.0f * viewportPosition.x) / windowSize.x - 1.0f,
        1.0f - (2.0f * viewportPosition.y) / windowSize.y, 
        1.0f
    };
    // clang-format on

    glm::vec4 rayClip{rayNdc.x, rayNdc.y, -1.0f, 1.0f};
    glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;

    rayEye.z = -1.0f;
    rayEye.w = 0.0f;

    return glm::normalize(glm::vec3(glm::inverse(viewMatrix) * rayEye));
}

}  // namespace sl::math