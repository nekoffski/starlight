#pragma once

#include <memory>
#include <vector>

#include "Mesh.h"

namespace sl::geometry {

struct Model {
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string path;
    std::string directory;
};
}