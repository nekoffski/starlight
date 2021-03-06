#pragma once

#include <memory>
#include <vector>

#include "Mesh.h"
#include "sl/core/GameObject.h"

namespace sl::geom {

struct Model : public core::GameObject {
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string path;
    std::string directory;
};
}
