#pragma once

#include <vector>

#include "sl/gfx/buffer/VertexArray.h"

namespace sl::physx {

struct BoundingBox {
    virtual std::shared_ptr<gfx::buffer::VertexArray> getVertexArray() const = 0;
    virtual std::vector<math::Vec3> getVertices() const = 0;
};

}