#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/math/Vertex.hh"
#include "starlight/renderer/core/RenderResource.hh"
#include "starlight/renderer/core/Shader.hh"

namespace sl {

struct PrimitiveUploadData {
    std::vector<Vertex3> vertices;
    std::vector<u32> indices;
};

struct Primitive {
    DeviceBufferHandle vertexBuffer;
    DeviceBufferHandle indexBuffer;
    u32 vertexBufferOffset;
    u32 indexBufferOffset;
    u32 indexCount;
};

}  // namespace sl
