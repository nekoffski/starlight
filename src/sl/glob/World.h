#pragma once

#include "sl/math/Vector.hpp"

namespace sl::glob {

struct World {
    math::Vec3 gravity = {0.0f, 10.0f, 0.0f};
};

}  // namespace sl::glob