#pragma once

#include <variant>

#include "starlight/renderer/rhi/RenderResource.hh"

namespace sl {

struct TextureRenderOutput {
    TextureHandle handle;
};

struct SurfaceRenderOutput {
    SurfaceHandle handle;
};

using RenderOutput = std::variant<TextureRenderOutput, SurfaceRenderOutput>;

}  // namespace sl
