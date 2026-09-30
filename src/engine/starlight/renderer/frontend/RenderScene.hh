#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/renderer/core/Primitive.hh"
#include "starlight/renderer/core/Shader.hh"

namespace sl {

struct RenderItem {
    std::vector<Primitive> primitives;
    ShaderHandle shader;
};

struct RenderScene {
    std::vector<RenderItem> items;
};

}  // namespace sl
