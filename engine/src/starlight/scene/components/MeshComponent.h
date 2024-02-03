#pragma once

#include "starlight/core/Core.h"

#include "starlight/renderer/Mesh.h"

namespace sl {

struct MeshComponent {
    u64 id;
    u64 entityId;

    Mesh* mesh;
};

}  // namespace sl