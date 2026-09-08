#include "Renderer.hh"

#include "starlight/core/Functional.hh"
#include "starlight/renderer/rhi/RenderResource.hh"

namespace sl {

Renderer::Renderer(const Config& config, RenderDevice& device)
    : m_config(config), m_device(device) {}

bool Renderer::tick() {
    if (m_pendingRequests.empty()) {
        return false;
    }
    tryToSubmitFrame();
    return true;
}

void Renderer::flush() {
    for (;;) {
        if (not tick()) {
            break;
        }
    }
}

Result<void> Renderer::submit(const RenderRequest& request) {
    if (m_pendingRequests.size() >= m_config.renderer.maxFramesInFlight) {
        return Error::unexpected(
            ErrorCode::tooManyFrameRequests, "Too many frame requests"
        );
    }
    m_pendingRequests.push(request);
    return {};
}

Result<RenderTarget> Renderer::createTarget(
    std::shared_ptr<RenderSurfaceProvider> surfaceProvider
) {
    auto surface = m_device.attachSurface(std::move(surfaceProvider));

    if (not surface) {
        return Error::unexpected(
            ErrorCode::surfaceCreationFailed,
            "Failed to create render surface: {}", surface.error().message()
        );
    }

    return RenderTarget{*surface};
}

void Renderer::destroyTarget(RenderTarget target) {
    std::visit(
        Overloader{
            [&](SurfaceHandle surface) { m_device.destroySurface(surface); },
            [&](TextureHandle) {
                log::panic("Destroying texture render output is not supported");
            }
        },
        target
    );
}

u64 Renderer::frameIndex() const {
    return m_frameNumber % m_config.renderer.maxFramesInFlight;
}

void Renderer::tryToSubmitFrame() {
    const auto& request = m_pendingRequests.front();
    NAMED_DEFER(requestDropper) { m_pendingRequests.pop(); };

    auto res = m_device.trySubmitFrame([&](RenderFrameRecorder& context) {
        return recordFrame(context, request);
    });

    if (not res) [[unlikely]] {
        if (res.error().code() != ErrorCode::tooManyFramesInFlight) [[likely]] {
            log::error("Failed to record frame: {}", res.error().message());
        } else {
            requestDropper.dismiss();
        }
        return;
    }
    m_frameNumber++;
}

Result<void> Renderer::recordFrame(
    RenderFrameRecorder& recorder, const RenderRequest& request
) {
    for (const auto& view : request.views) {
        ColorAttachment attachment{
            .target = view.target,
            .loadOp = LoadOp::clear,
            .storeOp = StoreOp::store,
            .clearColor = request.clearColor
        };

        RenderPassDescription pass{
            .label = "Main Pass", .colorAttachments = std::span(&attachment, 1)
        };

        auto res = recorder.renderPass(
            [](RenderPassEncoder&) -> Result<void> { return {}; }, pass
        );

        if (not res) {
            if (res.error().code() == ErrorCode::renderSurfaceNotDrawable) {
                continue;
            }

            return Error::unexpected(res.error());
        }
    }
    return {};
}

}  // namespace sl
