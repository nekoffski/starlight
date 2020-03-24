#pragma once

#include <memory>
#include <vector>

#include <starlight/geometry/Mesh.h>

namespace starl::geometry {

struct Model {
    std::vector<std::shared_ptr<Mesh>> meshes;
};
}