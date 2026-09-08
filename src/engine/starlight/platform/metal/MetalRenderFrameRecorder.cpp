#include "MetalRenderFrameRecorder.hh"

#include "MetalRenderPass.hh"

namespace sl {

MetalRenderFrameRecorder::MetalRenderFrameRecorder(
    MetalResourcePool& resourcePool, MTL::CommandBuffer* commandBuffer
)
    : m_resourcePool(resourcePool), m_commandBuffer(commandBuffer) {}

Result<CA::MetalDrawable*> MetalRenderFrameRecorder::acquireSurface(
    SurfaceHandle handle
) {
    for (const auto& acquired : m_acquiredSurfaces) {
        if (acquired.handle == handle) {
            return acquired.drawable;
        }
    }

    auto layer = m_resourcePool.getSurface(handle);

    if (not layer) {
        return Error::unexpected(layer.error());
    }

    auto* drawable = (*layer)->nextDrawable();

    if (not drawable) {
        return Error::unexpected(
            ErrorCode::renderSurfaceNotDrawable,
            "Failed to acquire render surface drawable"
        );
    }

    m_acquiredSurfaces.push_back({handle, drawable});
    return drawable;
}

Result<MTL::Texture*> MetalRenderFrameRecorder::resolveTarget(
    const RenderTarget& target
) {
    if (const auto* surface = std::get_if<SurfaceHandle>(&target)) {
        auto drawable = acquireSurface(*surface);

        if (not drawable) {
            return Error::unexpected(drawable.error());
        }

        return (*drawable)->texture();
    }

    return Error::unexpected(
        ErrorCode::renderEncodingFailed,
        "Metal texture render targets are not supported"
    );
}

Result<void> MetalRenderFrameRecorder::renderPass(
    const RenderPassDescription& description, const RenderPassCallback& record
) {
    auto* renderPass = MTL::RenderPassDescriptor::renderPassDescriptor();

    for (u32 index = 0u; index < description.colorAttachments.size(); ++index) {
        const auto& source = description.colorAttachments[index];
        auto texture = resolveTarget(source.target);

        if (not texture) {
            return Error::unexpected(texture.error());
        }

        auto* attachment = renderPass->colorAttachments()->object(index);

        attachment->setTexture(*texture);
        attachment->setLoadAction(toMetal(source.loadOp));
        attachment->setStoreAction(toMetal(source.storeOp));
        attachment->setClearColor(
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
