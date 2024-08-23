#pragma once

#include "Component.hh"

#include "starlight/core/Core.hh"
#include "starlight/renderer/PointLight.hh"

#include "starlight/scene/fwd.hh"

namespace sl {

struct PointLightComponent : Component {
    explicit PointLightComponent(u64 id, u64 entityId);
    explicit PointLightComponent(
      u64 id, u64 entityId, const Vec4<f32>& position, const Vec4<f32>& color,
      const Vec4<f32>& attenuation
    );

    kc::json::Node serialize() const override;
    static void deserialize(Entity* entity, const kc::json::Node& root);

    PointLight light;
};

}  // namespace sl
