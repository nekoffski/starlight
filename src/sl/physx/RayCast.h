#pragma once

#include <kc/math/Ray.h>

#include <optional>
#include <string>

#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::physx {

struct FindRayIntersectionArgs {
    mutable scene::components::TransformComponent::View transforms;
    mutable scene::components::ModelComponent::View models;
    const kc::math::Ray& primaryRay;
};

std::optional<std::string> findRayIntersection(const FindRayIntersectionArgs& args);

}  // namespace sl::physx