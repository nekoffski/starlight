#pragma once

#include "Component.hh"

#include "starlight/core/Core.hh"
#include "starlight/renderer/Material.hh"

#include "starlight/scene/fwd.hh"

namespace sl {

struct MaterialComponent : Component {
    explicit MaterialComponent(u64 id, u64 entityId, Material* material = nullptr);

    kc::json::Node serialize() const override;
    static void deserialize(Entity* entity, const kc::json::Node& root);

    Material* material;
};

}  // namespace sl
