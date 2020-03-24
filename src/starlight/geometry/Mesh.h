#pragma once

#include <memory>
#include <vector>

#include <starlight/geometry/Vertex.h>

namespace starl::platform {
namespace texture {
    class Texture;
}

namespace gpu {
    class VertexArray;
}
}

using namespace starl::platform;

namespace starl::geometry {

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<texture::Texture>> textures;
    std::shared_ptr<gpu::VertexArray> vertexArray;
};

using MeshPtr = std::shared_ptr<Mesh>;

} // namespace
