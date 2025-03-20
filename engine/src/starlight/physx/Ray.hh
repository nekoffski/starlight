#pragma once

#include "starlight/core/Core.hh"
#include "starlight/core/math/Vec.hh"

namespace sl {

struct Ray {
    Vec3<f32> origin;
    Vec3<f32> direction;
};

}  // namespace sl
