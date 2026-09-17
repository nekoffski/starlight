#pragma once

#include <memory>

#include "Metal.hh"
#include "MetalShader.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/rhi/GraphicsPipeline.hh"

namespace sl {

class MetalGraphicsPipeline : public NonCopyable, public NonMovable {
    struct Guard {};

   public:
    static Result<std::unique_ptr<MetalGraphicsPipeline>> create(
        MetalContext& ctx, MetalShader& shader,
        const GraphicsPipelineDescription& description
    );

    explicit MetalGraphicsPipeline(
        MTL::RenderPipelineState* pipelineState, Guard
    );
    ~MetalGraphicsPipeline();

    MTL::RenderPipelineState& state();

   private:
    MTL::RenderPipelineState* m_pipelineState;
};

}  // namespace sl
