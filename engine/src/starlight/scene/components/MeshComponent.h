#pragma once

#include "Component.h"

#include "starlight/core/Core.h"
#include "starlight/renderer/gpu/Mesh.h"

#include "starlight/scene/fwd.h"

namespace sl {

struct MeshComponent : Component {
    explicit MeshComponent(u64 id, u64 entityId, Mesh* mesh = nullptr);

    kc::json::Node serialize() const override;
    static void deserialize(Entity* entity, const kc::json::Node& root);

    Mesh* mesh = nullptr;
};

}  // namespace sl