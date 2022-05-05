#pragma once

#include <unordered_map>

#include "sl/geom/Model.h"
#include "sl/gpu/VertexArray.h"

namespace sl::glob {

class Geometry {
    using MeshVaoPair = std::pair<std::shared_ptr<geom::Mesh>, std::shared_ptr<gpu::VertexArray>>;

   public:
    explicit Geometry();

    std::shared_ptr<gpu::VertexArray> frontSquareVAO;
    std::shared_ptr<geom::Mesh> frontSquareMesh;

    std::shared_ptr<gpu::VertexArray> upSquareVAO;
    std::shared_ptr<geom::Mesh> upSquareMesh;

    std::shared_ptr<gpu::VertexArray> cubeVAO;
    std::shared_ptr<geom::Mesh> cubeMesh;

    std::shared_ptr<gpu::VertexArray> lineVAO;

    std::unordered_map<std::string, std::shared_ptr<geom::Mesh>> meshes;

   private:
    void initSquares();
    void initCube();
    void initLine();

    MeshVaoPair initSquare(std::vector<geom::Vertex> vertices, const std::string& name);
};
}  // namespace sl::glob
