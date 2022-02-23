#pragma once

#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::physx {

struct Vector {
    math::Vec3 origin;
    math::Vec3 direction;
};

struct ColoredVector {
    Vector vector;
    math::Vec3 color;
};

}  // namespace sl::physx