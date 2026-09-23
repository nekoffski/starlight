#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/math/Vertex.hh"
#include "starlight/renderer/rhi/RenderResource.hh"

namespace sl {

struct PrimitiveUploadData {
    std::vector<Vertex3> vertices;
    std::vector<u32> indices;
};

struct Primitive {
    DeviceBufferHandle vertexBuffer;
    DeviceBufferHandle indexBuffer;
    u64 vertexBufferOffset;
    u64 indexBufferOffset;
    u64 indexCount;
};

}  // namespace sl
