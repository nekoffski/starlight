#pragma once

#include <span>

#include "Mesh.h"
#include "RenderMode.h"

namespace sl {

struct RenderPacket {
    std::span<Mesh> meshes;
    float deltaTime;
    RenderMode renderMode;
    u64 frameNumber;
};

}  // namespace sl
