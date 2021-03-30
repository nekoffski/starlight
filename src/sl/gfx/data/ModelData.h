#pragma once

#include <memory>

#include "sl/geom/Model.h"
#include "sl/math/Vector.hpp"

namespace sl::gfx::data {

struct ModelData {
    explicit ModelData(std::shared_ptr<geom::Model> model)
        : model(std::move(model)) {
    }

    std::shared_ptr<geom::Model> model;
    std::vector<math::Vec3> positions;
};
}