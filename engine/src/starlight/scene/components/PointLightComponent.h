#pragma once

#include "Component.h"

#include "starlight/core/Core.h"
#include "starlight/renderer/PointLight.h"

#include "starlight/scene/fwd.h"

namespace sl {

struct PointLightComponent : Component {
    explicit PointLightComponent(u64 id, u64 entityId);
    explicit PointLightComponent(
      u64 id, u64 entityId, const Vec4f& position, const Vec4f& color,
      const Vec4f& attenuation
    );

    kc::json::Node serialize() const override;
    static void deserialize(Entity* entity, const kc::json::Node& root);

    PointLight light;
};

}  // namespace sl
