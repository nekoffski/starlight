#pragma once

#include <kc/math/Ray.h>

#include <optional>

#include "fwd.h"
#include "sl/math/Matrix.hpp"

namespace sl::physx {

struct Collider {
    struct IntersectRecord {
        float tNear;
        float tFar;
    };

    virtual bool collideWith(AxisAlignedBoundingBox&, const math::Mat4&) = 0;
    virtual std::optional<IntersectRecord> intersectsWith(const kc::math::Ray& ray,
                                                          const math::Mat4& modelMatrix) = 0;
    virtual void setModelMatrix(const math::Mat4&) = 0;
};

}  // namespace sl::physx