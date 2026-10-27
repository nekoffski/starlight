#include "MetalGraphicsPipeline.hh"

#include "starlight/core/Functional.hh"
#include "starlight/renderer/core/Vertex.hh"

namespace sl {

namespace {

Result<void> setVertexDescriptor(
    const VertexLayout& layoutDescription,
    MTL::RenderPipelineDescriptor* descriptor
) {
    auto* layout = MTL::VertexDescriptor::vertexDescriptor();
    auto stride = LayoutHelper::calculateStride(layoutDescription);

    // single buffer as for now
    layout->layouts()->object(0)->setStride(stride);

    u64 offset = 0u;

    for (u64 i = 0; i < layoutDescription.size(); ++i) {
        auto* attribute = layout->attributes()->object(i);
        const auto size = LayoutHelper::valueTypeSize(layoutDescription[i]);

        attribute->setFormat(toMetal(layoutDescription[i]));
        attribute->setOffset(offset);
        attribute->setBufferIndex(0u);

        descriptor->colorAttachments()->object(i)->setPixelFormat(
            MTL::PixelFormatBGRA8Unorm
        );

        offset += size;
    }

    descriptor->setVertexDescriptor(layout);
    return {};
}

}  // namespace

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

    auto& layoutDescription = shader.description().vertexLayout;

    if (not layoutDescription.has_value()) [[unlikely]] {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Vertex layout is not specified"
        );
    }

    if (auto r = setVertexDescriptor(layoutDescription.value(), descriptor);
        not r) [[unlikely]] {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Failed to create vertex descriptor: {}", r.error()
        );
    }

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
