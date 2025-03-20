#pragma once

#include <optional>

#include "starlight/core/Utils.hh"
#include "starlight/physx/Ray.hh"

namespace sl {

struct BoundingVolume {
    virtual std::optional<Interval<f32>> intersects(const Ray& ray) const = 0;
};

}  // namespace sl
