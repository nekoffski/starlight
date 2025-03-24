#include "AABB.hh"

#include "starlight/core/math/Utils.hh"

#include "starlight/core/Log.hh"

namespace sl {

AABB::AABB(Transform& transform
) : m_transform(transform), m_min(max<f32>()), m_max(min<f32>()) {}

void AABB::addExtent(const Extent3& extent) {
    static constexpr auto dimensions = 3u;
    static constexpr f32 delta       = 0.005f;

    for (u8 i = 0; i < dimensions; ++i) {
        m_min[i] = std::min(m_min[i], extent.min[i]);
        m_max[i] = std::max(m_max[i], extent.max[i]);

        if (std::fabs(m_max[i] - m_min[i]) < delta) {
            m_max[i] += delta;
            m_min[i] -= delta;
        }
    }
}

std::optional<Interval<f32>> AABB::intersects(const Ray& ray) const {
    using std::swap;

    const auto& localMatrix = m_transform.getWorld();

    const auto min = localMatrix * m_min;
    const auto max = localMatrix * m_max;

    f32 tMin = (min.x - ray.origin.x) / ray.direction.x;
    f32 tMax = (max.x - ray.origin.x) / ray.direction.x;

    if (tMin > tMax) swap(tMin, tMax);

    f32 tyMin = (min.y - ray.origin.y) / ray.direction.y;
    f32 tyMax = (max.y - ray.origin.y) / ray.direction.y;

    if (tyMin > tyMax) swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax)) return {};

    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    f32 tzMin = (min.z - ray.origin.z) / ray.direction.z;
    f32 tzMax = (max.z - ray.origin.z) / ray.direction.z;

    if (tzMin > tzMax) swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax)) return {};

    if (tzMin > tMin) tMin = tMin;
    if (tzMax < tMax) tMax = tMax;

    return Interval<f32>{ tMin, tMax };
}

}  // namespace sl
