#pragma once

#include "starlight/core/Core.hh"
#include "starlight/math/Vec.hh"
#include "starlight/math/Extent.hh"
#include "starlight/physx/Transform.hh"

#include "BoundingVolume.hh"

namespace sl {

class AABB : public BoundingVolume {
public:
    explicit AABB();
    explicit AABB(Transform& transform);

    void addExtent(const Extent3& extent);

    std::optional<Interval<f32>> intersects(const Ray& ray) const override;
    std::type_index getType() const override;

    const Vec3<f32>& getMin() const;
    const Vec3<f32>& getMax() const;

private:
    Vec3<f32> m_min;
    Vec3<f32> m_max;
};

}  // namespace sl
