#pragma once

#include "fwd.h"
#include "sl/math/Matrix.hpp"

namespace sl::physx {

struct Collider {
    virtual bool collideWith(AxisAlignedBoundingBox&, const math::Mat4&) = 0;
    virtual void setModelMatrix(const math::Mat4&) = 0;
};

}  // namespace sl::physx