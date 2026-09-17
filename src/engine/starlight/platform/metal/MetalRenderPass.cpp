#include "MetalRenderPass.hh"

namespace sl {

MetalRenderPassEncoder::MetalRenderPassEncoder(
    MetalResourcePool& resourcePool, MTL::RenderCommandEncoder* encoder
)
    : m_resourcePool(resourcePool), m_encoder(encoder) {}

MetalRenderPassEncoder::~MetalRenderPassEncoder() { m_encoder->endEncoding(); }

Result<void> MetalRenderPassEncoder::setPipeline(
    GraphicsPipelineHandle handle
) {
    auto* pipeline = m_resourcePool.getGraphicsPipeline(handle);

    if (not pipeline) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid graphics pipeline handle"
        );
    }

    m_encoder->setRenderPipelineState(&pipeline->state());
    return {};
}

void MetalRenderPassEncoder::draw(u32 vertexCount) {
    m_encoder->drawPrimitives(
        MTL::PrimitiveType::PrimitiveTypeTriangle, 0u, vertexCount, 1u
    );
}

MTL::LoadAction toMetal(LoadOp op) {
    switch (op) {
        case LoadOp::load:
            return MTL::LoadActionLoad;
        case LoadOp::clear:
            return MTL::LoadActionClear;
        default:
            return MTL::LoadActionDontCare;
    }
}

MTL::StoreAction toMetal(StoreOp op) {
    switch (op) {
        case StoreOp::store:
            return MTL::StoreActionStore;
        default:
            return MTL::StoreActionDontCare;
    }
}

}  // namespace sl
