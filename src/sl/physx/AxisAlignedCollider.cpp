#include "AxisAlignedCollider.h"

#include <kc/core/Log.h>

#include "AxisAlignedBoundingBox.h"
#include "sl/math/Utils.hpp"

namespace sl::physx {

bool AxisAlignedCollider::collideWith(AxisAlignedBoundingBox& boundingBox,
                                      const math::Mat4& modelMatrix) {
    const auto min = modelMatrix * boundingBox.min;
    const auto max = modelMatrix * boundingBox.max;

    const auto lmin = m_modelMatrix * m_min;
    const auto lmax = m_modelMatrix * m_max;

    static constexpr float delta = 0.1f;

    return (lmin.x <= max.x && lmax.x >= min.x) && (lmin.y <= max.y && lmax.y >= min.y) &&
           (lmin.z <= max.z && lmax.z >= min.z);
}

std::optional<Collider::IntersectRecord> AxisAlignedCollider::intersectsWith(
    const kc::math::Ray& ray, const math::Mat4& modelMatrix) {
    const auto min = modelMatrix * m_min;
    const auto max = modelMatrix * m_max;

    using std::swap;

    const auto origin = ray.getOrigin();
    const auto direction = ray.getDirection();

    float tmin = (min.x - origin.x) / direction.x;
    float tmax = (max.x - origin.x) / direction.x;

    if (tmin > tmax) swap(tmin, tmax);

    float tymin = (min.y - origin.y) / direction.y;
    float tymax = (max.y - origin.y) / direction.y;

    if (tymin > tymax) swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return {};

    if (tymin > tmin) tmin = tymin;

    if (tymax < tmax) tmax = tymax;

    float tzmin = (min.z - origin.z) / direction.z;
    float tzmax = (max.z - origin.z) / direction.z;

    if (tzmin > tzmax) swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return {};

    if (tzmin > tmin) tmin = tzmin;

    if (tzmax < tmax) tmax = tzmax;

    return IntersectRecord{tmin, tmax};
}
}  // namespace sl::physx