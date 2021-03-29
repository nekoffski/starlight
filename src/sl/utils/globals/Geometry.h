#pragma once

#include "sl/geometry/Model.h"
#include "sl/graphics/buffer/VertexArray.h"

namespace sl::utils::globals {

class Geometry {
public:
    explicit Geometry();

    std::shared_ptr<graphics::buffer::VertexArray> squareVAO;
    std::shared_ptr<geometry::Model> squareModel;

private:
    void initSquare();
};
}
