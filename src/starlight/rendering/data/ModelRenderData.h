#pragma once

#include <memory>

#include <starlight/geometry/Model.h>
#include <starlight/math/Matrix.hpp>

namespace starl::rendering::data {

struct ModelRenderData {
    ModelRenderData(std::shared_ptr<geometry::Model> model)
        : model(std::move(model))
        , modelMatrix(std::make_shared<math::Mat4>(1.0f)) {}

    std::shared_ptr<geometry::Model> model;
    std::shared_ptr<math::Mat4> modelMatrix;
};
}