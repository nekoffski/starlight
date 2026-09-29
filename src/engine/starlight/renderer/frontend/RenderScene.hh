#pragma once

#include <vector>

#include "Primitive.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/rhi/Shader.hh"

namespace sl {

struct RenderItem {
    std::vector<Primitive> primitives;
    ShaderHandle shader;
};

struct RenderScene {
    std::vector<RenderItem> items;
};

}  // namespace sl
