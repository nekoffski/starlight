#include "Transforms.h"

namespace sl::math {

glm::vec3 viewportToWorld(const ViewportToWorldArgs& args) {
    // clang-format off
    glm::vec3 rayNdc {
        (2.0f * args.coordinates.x) / args.viewportSize.x - 1.0f,
        1.0f - (2.0f * args.coordinates.y) / args.viewportSize.y, 
        1.0f
    };
    // clang-format on

    glm::vec4 rayClip{rayNdc.x, rayNdc.y, -1.0f, 1.0f};
    glm::vec4 rayEye = glm::inverse(args.projectionMatrix) * rayClip;

    rayEye.z = -1.0f;
    rayEye.w = 0.0f;

    return glm::normalize(glm::vec3(glm::inverse(args.viewMatrix) * rayEye));
}

}  // namespace sl::math