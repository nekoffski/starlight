#pragma once

#include <kc/math/Ray.h>

#include <optional>
#include <string>

#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::physx {

struct RayCaster {
    struct Args {
        scene::components::TransformComponent::View transforms;
        scene::components::ModelComponent::View models;
        const kc::math::Ray& primaryRay;
    };

    static std::optional<std::string> findIntersection(Args&& args);
};

}  // namespace sl::physx