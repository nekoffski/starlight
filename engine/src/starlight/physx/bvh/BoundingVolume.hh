#pragma once

#include <optional>

#include <kstd/RTTI.hh>

#include "starlight/core/Utils.hh"
#include "starlight/physx/Ray.hh"
#include "starlight/physx/Transform.hh"

namespace sl {

struct BoundingVolume : kstd::RTTI, Transformable {
    virtual std::optional<Interval<f32>> intersects(const Ray& ray) const = 0;
};

}  // namespace sl
