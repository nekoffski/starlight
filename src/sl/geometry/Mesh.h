#pragma once

#include <memory>
#include <vector>

#include "Vertex.h"
#include "sl/graphics/Texture.h"
#include "sl/graphics/buffer/VertexArray.h"

namespace sl::geometry {

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<sl::graphics::Texture>> textures{};
    std::shared_ptr<graphics::buffer::VertexArray> vertexArray;
};

using MeshPtr = std::shared_ptr<Mesh>;

} // namespace
