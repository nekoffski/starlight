#pragma once

#include "sl/math/Matrix.hpp"

#include "fwd.h"

namespace sl::physx {

struct Collider {
    virtual bool collideWith(AxisAlignedBoundingBox&, const math::Mat4&) = 0;
    virtual void setModelMatrix(const math::Mat4&) = 0;
};

}