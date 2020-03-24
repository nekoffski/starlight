#pragma once

#include <memory>

#include <starlight/geometry/Model.h>
#include <starlight/math/Matrix.hpp>

namespace starl::rendering::renderable {

struct ModelRenderObject {
    std::shared_ptr<geometry::Model> model;
    std::shared_ptr<math::Mat4> modelMatrix;
};
}