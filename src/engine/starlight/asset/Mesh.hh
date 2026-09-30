#pragma once

#include "starlight/core/Core.hh"
#include "starlight/renderer/core/Primitive.hh"

namespace sl {

struct MeshPart {
    PrimitiveHandle primitive;
    // materialId
};

struct Mesh {
    std::vector<MeshPart> parts;
};

}  // namespace sl
