#pragma once

#include <optional>

#include "starlight/physx/Ray.hh"

namespace sl {

struct BoundingVolume {
    virtual std::optional<Intersection> intersects(const Ray& ray) const = 0;
};

}  // namespace sl
