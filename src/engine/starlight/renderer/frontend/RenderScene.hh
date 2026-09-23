#pragma once

#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/renderer/rhi/Shader.hh"

namespace sl {

struct RenderItem {
    u32 vertexCount;
    ShaderHandle shader;
};

struct RenderScene {
    std::vector<RenderItem> items;
};

}  // namespace sl
