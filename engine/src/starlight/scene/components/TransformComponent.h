#pragma once

#include "Component.h"

#include "starlight/core/Core.h"
#include "starlight/core/math/Transform.h"

#include "starlight/scene/fwd.h"

namespace sl {

struct TransformComponent : Component {
    explicit TransformComponent(u64 id, u64 entityId);
    explicit TransformComponent(
      u64 id, u64 entityId, const Vec3<f32>& position, const Vec3<f32>& scale,
      const Mat4<f32>& rotation
    );

    kc::json::Node serialize() const override;
    static void deserialize(Entity* entity, const kc::json::Node& root);

    Transform transform;
};

}  // namespace sl
