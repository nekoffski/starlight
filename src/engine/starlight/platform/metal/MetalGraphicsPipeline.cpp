#include "MetalGraphicsPipeline.hh"

#include "starlight/core/Functional.hh"

namespace sl {

Result<std::unique_ptr<MetalGraphicsPipeline>> MetalGraphicsPipeline::create(
    MetalContext& ctx, MetalShader& shader,
    const GraphicsPipelineDescription& description
) {
    if (description.format != TextureFormat::bgra8unorm) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Unsupported texture format"
        );
    }

    auto* vertexStage = shader.function(ShaderStage::vertex);
    auto* fragmentStage = shader.function(ShaderStage::fragment);

    if (not vertexStage || not fragmentStage) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Graphics shader requires both vertex and fragment stages"
        );
    }

    auto* pool = NS::AutoreleasePool::alloc()->init();
    DEFER { pool->release(); };

    auto* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    DEFER { descriptor->release(); };

    descriptor->setVertexFunction(vertexStage);
    descriptor->setFragmentFunction(fragmentStage);

    descriptor->colorAttachments()->object(0)->setPixelFormat(
        MTL::PixelFormatBGRA8Unorm
    );

    NS::Error* error = nullptr;
    auto* pipelineState =
        ctx.device().newRenderPipelineState(descriptor, &error);

    if (not pipelineState) {
        return Error::unexpected(
            ErrorCode::metalError, "Failed to create render pipeline state: {}",
            parseMetalError(error)
        );
    }

    return std::make_unique<MetalGraphicsPipeline>(pipelineState, Guard{});
}

MetalGraphicsPipeline::MetalGraphicsPipeline(
    MTL::RenderPipelineState* pipelineState, Guard
)
    : m_pipelineState(pipelineState) {}

MetalGraphicsPipeline::~MetalGraphicsPipeline() { m_pipelineState->release(); }

MTL::RenderPipelineState& MetalGraphicsPipeline::state() {
    return *m_pipelineState;
}

}  // namespace sl
