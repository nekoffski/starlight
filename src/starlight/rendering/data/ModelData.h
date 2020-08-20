#pragma once

#include <memory>

#include "starlight/geometry/Model.h"
#include "starlight/math/Vector.hpp"

namespace sl::rendering::data {

struct ModelData {
    explicit ModelData(std::shared_ptr<geometry::Model> model)
        : model(std::move(model)) {
    }

    std::shared_ptr<geometry::Model> model;
    std::vector<math::Vec3> positions;
};
}