#pragma once

#include "Component.h"

#include "starlight/core/Core.h"
#include "starlight/renderer/Material.h"

namespace sl {

struct MaterialComponent : Component {
    explicit MaterialComponent(u64 id, u64 entityId, Material* material = nullptr);

    kc::json::Node serialize() const override;

    Material* material;
};

}  // namespace sl
