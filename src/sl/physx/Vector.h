#pragma once

#include "sl/math/Vector.hpp"

namespace sl::physx {

struct Vector {
    math::Vec3 origin;
    math::Vec3 direction;
};

}