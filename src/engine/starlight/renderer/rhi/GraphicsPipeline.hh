#pragma once

#include "RenderResource.hh"
#include "Texture.hh"

namespace sl {

struct GraphicsPipelineDescription {
    TextureFormat format;
    ShaderHandle shader;

    bool operator==(const GraphicsPipelineDescription&) const = default;
};

}  // namespace sl
