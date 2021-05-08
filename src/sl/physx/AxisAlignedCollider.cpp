#include "AxisAlignedCollider.h"

#include "AxisAlignedBoundingBox.h"

namespace sl::physx {

bool AxisAlignedCollider::collideWith(AxisAlignedBoundingBox& boundingBox, const math::Mat4& modelMatrix) {
    const auto min = modelMatrix * math::Vec4(boundingBox.min, 1.0f);
    const auto max = modelMatrix * math::Vec4(boundingBox.max, 1.0f);

    const auto lmin = m_modelMatrix * math::Vec4(m_min, 1.0f);
    const auto lmax = m_modelMatrix * math::Vec4(m_max, 1.0f);

    constexpr float delta = 0.1f;

    // clang-format off
    return (lmin.x <= max.x + delta && lmax.x + delta >= min.x) &&
           (lmin.y <= max.y + delta && lmax.y + delta >= min.y) &&
           (lmin.z <= max.z + delta && lmax.z + delta >= min.z);
    // clang-format on;
}
}