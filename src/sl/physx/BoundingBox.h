#pragma once

#include <vector>

#include "geom/Vertex.h"
#include "gfx/buffer/VertexArray.h"

namespace sl::physx {

struct BoundingBox {
    virtual std::shared_ptr<gfx::buffer::VertexArray> getVertexArray() const = 0;
    virtual std::vector<geom::Vertex> getVertices() const = 0;
};

}