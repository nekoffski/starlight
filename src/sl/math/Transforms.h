#pragma once

#include <glm/glm.hpp>

namespace sl::math {

struct ViewportToWorldArgs {
    const glm::vec2& coordinates;
    const glm::mat4& viewMatrix;
    const glm::mat4& projectionMatrix;
    const glm::vec2& viewportSize;
};

glm::vec3 viewportToWorld(const ViewportToWorldArgs& args);

}  // namespace sl::math