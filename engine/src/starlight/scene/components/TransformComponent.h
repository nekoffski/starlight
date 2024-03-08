#pragma once

#include "Component.h"

#include "starlight/core/Core.h"
#include "starlight/core/math/Transform.h"

namespace sl {

struct TransformComponent : Component {
    explicit TransformComponent(u64 id, u64 entityId);

    kc::json::Node serialize() const override;

    Transform transform;
};

}  // namespace sl
