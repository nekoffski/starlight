#pragma once

#include <memory>

#include "starlight/geometry/Model.h"
#include "starlight/math/Matrix.hpp"

namespace sl::rendering::data {

struct ModelData {
    explicit ModelData(std::shared_ptr<geometry::Model> model)
        : model(std::move(model)) {
    }

    std::shared_ptr<geometry::Model> model;
    std::vector<math::Mat4> modelMatrices;
};
}