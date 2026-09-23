#pragma once

#include "starlight/core/Core.hh"
#include "starlight/renderer/frontend/Primitive.hh"

namespace sl {

struct MeshPart {
    Primitive primitive;
    // materialId
};

struct Mesh {
    std::vector<MeshPart> parts;
};

}  // namespace sl
