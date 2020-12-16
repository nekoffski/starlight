#pragma once

#include <memory>

#include "sl/geometry/Model.h"
#include "sl/math/Vector.hpp"

namespace sl::graphics::data {

struct ModelData {
    explicit ModelData(std::shared_ptr<geometry::Model> model)
        : model(std::move(model)) {
    }

    std::shared_ptr<geometry::Model> model;
    std::vector<math::Vec3> positions;
};
}