#pragma once

#include "starlight/core/Core.hh"
#include "starlight/renderer/core/ShaderDataLayout.hh"

namespace sl {

// Owned CPU data; independent of asset references and uploaded primitives.
struct GeometryData {
    VertexLayout vertexLayout;
    Bytes vertexData;
    Bytes indexData;
    u32 vertexCount{0};
    u32 indexCount{0};
};

}  // namespace sl
