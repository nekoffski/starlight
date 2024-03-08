#pragma once

#include "Component.h"

#include "starlight/core/Core.h"
#include "starlight/renderer/PointLight.h"

namespace sl {

struct PointLightComponent : Component {
    explicit PointLightComponent(u64 id, u64 entityId);

    kc::json::Node serialize() const override;

    PointLight light;
};

}  // namespace sl
