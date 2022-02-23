#include "AxisAlignedCollider.h"

#include "AxisAlignedBoundingBox.h"
#include "sl/math/Utils.hpp"

namespace sl::physx {

bool AxisAlignedCollider::collideWith(AxisAlignedBoundingBox& boundingBox,
                                      const math::Mat4& modelMatrix) {
    const auto min = modelMatrix * boundingBox.min;
    const auto max = modelMatrix * boundingBox.max;

    const auto lmin = m_modelMatrix * m_min;
    const auto lmax = m_modelMatrix * m_max;

    constexpr float delta = 0.1f;

    // clang-format off
    return (lmin.x <= max.x && lmax.x >= min.x) &&
           (lmin.y <= max.y && lmax.y >= min.y) &&
           (lmin.z <= max.z && lmax.z >= min.z);
    // clang-format on;
}
}