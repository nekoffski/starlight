#pragma once

#include <span>

#include "Mesh.h"

namespace sl {

struct RenderPacket {
    std::span<Mesh> meshes;
};

}  // namespace sl
