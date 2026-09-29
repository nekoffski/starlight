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

void MetalRenderPassEncoder::drawIndexed(u32 indexCount, u32 instanceCount) {
    u64 indexBufferOffset = 0u;
    MTL::Buffer* indexBuffer = nullptr;

    if (m_indexBuffer) {
        indexBuffer = &m_indexBuffer->buffer->handle();
        indexBufferOffset = m_indexBuffer->offset;
    }

    m_encoder->drawIndexedPrimitives(
        MTL::PrimitiveType::PrimitiveTypeTriangle, indexCount,
        MTL::IndexType::IndexTypeUInt32, indexBuffer, indexBufferOffset,
        instanceCount
    );
}

Result<void> MetalRenderPassEncoder::setVertexBuffer(
    const DeviceBufferSlice& slice
) {
    auto* vertexBuffer = m_resourcePool.getBuffer(slice.handle);

    if (not vertexBuffer) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid vertex buffer handle"
        );
    }

    m_encoder->setVertexBuffer(&vertexBuffer->handle(), slice.offset, 0);
    return {};
}

Result<void> MetalRenderPassEncoder::setIndexBuffer(
    const DeviceBufferSlice& slice
) {
    auto* indexBuffer = m_resourcePool.getBuffer(slice.handle);

    if (not indexBuffer) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid index buffer handle"
        );
    }

    m_indexBuffer.emplace(indexBuffer, slice.offset);
    return {};
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
