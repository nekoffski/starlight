#pragma once

#include <memory>
#include <vector>

#include "Vertex.h"

namespace sl::platform {
namespace texture {
    class Texture;
}

namespace gpu {
    class VertexArray;
}
}

namespace sl::geometry {

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<platform::texture::Texture>> textures{};
    std::shared_ptr<platform::gpu::VertexArray> vertexArray;
};

using MeshPtr = std::shared_ptr<Mesh>;

} // namespace
