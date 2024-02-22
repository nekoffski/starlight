#pragma once

#include "starlight/core/Core.h"

#include "starlight/renderer/gpu/Mesh.h"

namespace sl {

struct MeshComponent {
    u64 id;
    u64 entityId;

    Mesh* mesh = nullptr;
};

}  // namespace sl