#pragma once

#include <unordered_map>

#include "sl/geom/Model.h"
#include "sl/gfx/buffer/VertexArray.h"

namespace sl::utils::globals {

class Geometry {
public:
    explicit Geometry();

    std::shared_ptr<gfx::buffer::VertexArray> squareVAO;
    std::shared_ptr<geom::Mesh> squareMesh;

    std::unordered_map<std::string, std::shared_ptr<geom::Mesh>> meshes;

private:
    void initSquare();
};
}
