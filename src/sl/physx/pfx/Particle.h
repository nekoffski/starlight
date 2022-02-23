#pragma once

#include "sl/math/Vector.hpp"

namespace sl::physx::pfx {
struct Particle {
    math::Vec3 position;
    math::Vec3 color;
    math::Vec3 direction;
    math::Vec3 deltaDirection;

    float scale;
    float deltaScale;
    float speed;
    float ttl;
    float directionFactor;
};
}  // namespace sl::physx::pfx
