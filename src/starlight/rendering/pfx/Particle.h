#pragma once

#include "starlight/math/Vector.hpp"

namespace sl::rendering::pfx {

struct Particle {
    math::Vec3 position;
    math::Vec3 color;
    math::Vec3 direction;
    float scale;
    float speed;
    float ttl;
};
}
