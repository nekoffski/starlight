#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/math/Vec.hh"
#include "starlight/core/math/Extent.hh"
#include "starlight/core/math/Transform.hh"

#include "BoundingVolume.hh"

namespace sl {

class AABB : public BoundingVolume {
public:
    explicit AABB(Transform& transform);

    void addExtent(const Extent3& extent);
    std::optional<Interval<f32>> intersects(const Ray& ray) const override;

private:
    Transform& m_transform;

    Vec3<f32> m_min;
    Vec3<f32> m_max;
};

}  // namespace sl
