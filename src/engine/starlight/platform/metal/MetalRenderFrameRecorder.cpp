#include "MetalRenderFrameRecorder.hh"

#include "MetalRenderPass.hh"

namespace sl {

MetalRenderFrameRecorder::MetalRenderFrameRecorder(
    MetalResourcePool& resourcePool, MTL::CommandBuffer* commandBuffer
)
    : m_resourcePool(resourcePool), m_commandBuffer(commandBuffer) {}

Result<RenderFrameSurfaceImage> MetalRenderFrameRecorder::acquireSurface(
    SurfaceHandle handle
) {
    auto layer = m_resourcePool.getSurface(handle);

    if (not layer) {
        return Error::unexpected(layer.error());
    }

    auto* drawable = layer.value()->nextDrawable();

    if (not drawable) {
        return Error::unexpected(
            ErrorCode::renderSurfaceNotDrawable,
            "Failed to acquire render surface drawable"
        );
    }

    m_acquiredSurfaces.push_back({handle, drawable});
    return RenderFrameSurfaceImage{.token = m_acquiredSurfaces.size() - 1};
}

Result<void> MetalRenderFrameRecorder::renderPass(
    RenderPassCallback record, const RenderPassDescription& description
) {
    auto* renderPass = MTL::RenderPassDescriptor::renderPassDescriptor();

    for (u32 index = 0u; index < description.colorAttachments.size(); ++index) {
        const auto& source = description.colorAttachments[index];

        auto* drawable = m_acquiredSurfaces[source.image.token].drawable;
        auto* target = renderPass->colorAttachments()->object(index);

        target->setTexture(drawable->texture());
        target->setLoadAction(toMetal(source.loadOp));
        target->setStoreAction(toMetal(source.storeOp));
        target->setClearColor(
            MTL::ClearColor{
                source.clearColor.r, source.clearColor.g, source.clearColor.b,
                source.clearColor.a
            }
        );
    }

    auto* nativeEncoder = m_commandBuffer->renderCommandEncoder(renderPass);

    if (not nativeEncoder) {
        return Error::unexpected(
            ErrorCode::renderEncodingFailed,
            "Failed to create render command encoder"
        );
    }

    MetalRenderPassEncoder encoder{nativeEncoder};
    return record(encoder);
}

void MetalRenderFrameRecorder::schedulePresentations() {
    for (const auto& surface : m_acquiredSurfaces) {
        m_commandBuffer->presentDrawable(surface.drawable);
    }
}

}  // namespace sl
