#pragma once

#include <unordered_map>

#include "sl/geom/Model.h"
#include "sl/gfx/buffer/VertexArray.h"

namespace sl::utils::globals {

class Geometry {
    using MeshVaoPair = std::pair<std::shared_ptr<geom::Mesh>, std::shared_ptr<gfx::buffer::VertexArray>>;

public:
    explicit Geometry();

    std::shared_ptr<gfx::buffer::VertexArray> frontSquareVAO;
    std::shared_ptr<geom::Mesh> frontSquareMesh;

    std::shared_ptr<gfx::buffer::VertexArray> upSquareVAO;
    std::shared_ptr<geom::Mesh> upSquareMesh;

    std::shared_ptr<gfx::buffer::VertexArray> cubeVAO;
    std::shared_ptr<geom::Mesh> cubeMesh;

    std::shared_ptr<gfx::buffer::VertexArray> lineVAO;

    std::unordered_map<std::string, std::shared_ptr<geom::Mesh>> meshes;

private:
    void initSquares();
    void initCube();
    void initLine();

    MeshVaoPair initSquare(std::vector<geom::Vertex> vertices, const std::string& name);
};
}
