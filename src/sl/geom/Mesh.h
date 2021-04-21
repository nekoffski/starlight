#pragma once

#include <memory>
#include <vector>

#include "Vertex.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/VertexArray.h"

namespace sl::geom {

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<sl::gfx::Texture>> textures;
    std::shared_ptr<gfx::buffer::VertexArray> vertexArray;

    std::string name = "ExampleMesh";
};

using MeshPtr = std::shared_ptr<Mesh>;

} // namespace
