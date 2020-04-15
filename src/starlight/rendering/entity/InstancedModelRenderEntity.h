#pragma once

#include <memory>
#include <vector>

#include <starlight/geometry/Model.h>
#include <starlight/math/Matrix.hpp>

namespace starl::rendering::entity {

struct InstancedModelRenderEntity {
    std::shared_ptr<geometry::Model> model;
    std::vector<std::shared_ptr<math::Mat4>> modelMatrices;
};
}