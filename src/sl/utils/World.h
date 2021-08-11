#pragma once

#include "sl/math/Vector.hpp"

namespace sl::utils {

struct World {
    math::Vec3 gravity = { 0.0f, 10.0f, 0.0f };
};

}