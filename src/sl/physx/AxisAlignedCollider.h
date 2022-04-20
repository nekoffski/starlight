#pragma once

#include "Collider.h"
#include "fwd.h"
#include "sl/math/Vector.hpp"

namespace sl::physx {

class AxisAlignedCollider : public Collider {
   public:
    explicit AxisAlignedCollider(const math::Vec3& min, const math::Vec3& max)
        : m_min(min), m_max(max), m_modelMatrix(math::identityMatrix) {}

    bool collideWith(AxisAlignedBoundingBox&, const math::Mat4&) override;
    std::optional<Collider::IntersectRecord> intersectsWith(const kc::math::Ray& ray,
                                                            const math::Mat4& modelMatrix) override;

    void setModelMatrix(const math::Mat4& modelMatrix) override { m_modelMatrix = modelMatrix; }

   private:
    math::Vec3 m_min;
    math::Vec3 m_max;

    math::Mat4 m_modelMatrix;
};

}  // namespace sl::physx