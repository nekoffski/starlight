#pragma once

#include "Component.hh"

#include "starlight/core/Core.hh"
#include "starlight/renderer/gpu/Mesh.hh"

#include "starlight/scene/fwd.hh"

namespace sl {

struct MeshComponent : Component {
    explicit MeshComponent(u64 id, u64 entityId, Mesh* mesh = nullptr);

    kc::json::Node serialize() const override;
    static void deserialize(Entity* entity, const kc::json::Node& root);

    Mesh* mesh = nullptr;
};

}  // namespace sl