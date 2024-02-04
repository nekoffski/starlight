#pragma once

#include "starlight/core/Core.h"
#include "starlight/core/math/Transform.h"

namespace sl {

struct TransformComponent {
    u64 id;
    u64 entityId;

    Transform transform;
};

}  // namespace sl
