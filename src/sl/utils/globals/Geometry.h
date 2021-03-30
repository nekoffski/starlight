#pragma once

#include "sl/geom/Model.h"
#include "sl/gfx/buffer/VertexArray.h"

namespace sl::utils::globals {

class Geometry {
public:
    explicit Geometry();

    std::shared_ptr<gfx::buffer::VertexArray> squareVAO;
    std::shared_ptr<geom::Model> squareModel;

private:
    void initSquare();
};
}
