#include "MetalGraphicsPipeline.hh"

#include <kcore/Functional.hh>

namespace sl {

Result<NS::SharedPtr<MTL::VertexDescriptor>> createMetalVertexDescriptor(
    const VertexLayout& layoutDescription
) {
    const auto r = validateVertexLayout(layoutDescription);
    if (not r) {
        return Error::unexpected(r.error());
    }
    auto layout = NS::TransferPtr(MTL::VertexDescriptor::alloc()->init());
    for (const auto& input : layoutDescription.attributes) {
        const auto format = toMetal(input.type);
        if (not format) {
            return Error::unexpected(format.error());
        }
        auto* attribute = layout->attributes()->object(input.location);

        attribute->setFormat(*format);
        attribute->setOffset(input.offset);
        attribute->setBufferIndex(metalVertexBufferIndex);
    }

    layout->layouts()
        ->object(metalVertexBufferIndex)
        ->setStride(layoutDescription.stride);
    return layout;
}

Result<std::unique_ptr<MetalGraphicsPipeline>> MetalGraphicsPipeline::create(
    MetalContext& ctx, MetalShader& shader,
    const GraphicsPipelineDescription& description
) {
    if (description.format != TextureFormat::bgra8unorm) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument, "Unsupported texture format"
        );
    }

    auto* vertexStage = shader.function(ShaderStage::vertex);
    auto* fragmentStage = shader.function(ShaderStage::fragment);

    if (not vertexStage || not fragmentStage) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument,
            "Graphics shader requires both vertex and fragment stages"
        );
    }

    auto* pool = NS::AutoreleasePool::alloc()->init();
    KCORE_DEFER { pool->release(); };

    auto* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    KCORE_DEFER { descriptor->release(); };

    descriptor->setVertexFunction(vertexStage);
    descriptor->setFragmentFunction(fragmentStage);

    const auto& inputs = shader.description().vertexInputs;
    if (not inputs) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument,
            "Shader does not declare vertex inputs"
        );
    }
    const auto r =
        validateVertexCompatibility(description.vertexLayout, *inputs);
    if (not r) {
        return Error::unexpected(r.error());
    }
    const auto layout = createMetalVertexDescriptor(description.vertexLayout);
    if (not layout) {
        return Error::unexpected(layout.error());
    }
    descriptor->setVertexDescriptor(layout->get());
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
