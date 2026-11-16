#pragma once

#include "starlight/renderer/core/RenderResource.hh"
#include "starlight/renderer/core/ShaderDataLayout.hh"
#include "starlight/renderer/core/Texture.hh"

namespace sl {

struct GraphicsPipelineDescription {
    TextureFormat format;
    ShaderHandle shader;
    VertexLayout vertexLayout;

    bool operator==(const GraphicsPipelineDescription&) const = default;
};

}  // namespace sl
