#include "Renderer.hh"

#include "starlight/core/Functional.hh"
#include "starlight/renderer/rhi/RenderResource.hh"

namespace sl {

Renderer::Renderer(const Config& config, RenderDevice& device)
    : m_config(config), m_device(device) {}

bool Renderer::tick() { return false; }

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
            ErrorCode::tooManyFramesInFlight, "Too many frames in flight"
        );
    }
    m_pendingRequests.push(request);
    return {};
}

Result<RenderOutput> Renderer::createOutput(
    std::shared_ptr<RenderSurfaceProvider> surfaceProvider
) {
    auto surface = m_device.attachSurface(std::move(surfaceProvider));

    if (not surface) {
        return Error::unexpected(
            ErrorCode::surfaceCreationFailed,
            "Failed to create render surface: {}", surface.error().message()
        );
    }

    return SurfaceRenderOutput{*surface};
}

void Renderer::destroyOutput(RenderOutput output) {
    std::visit(
        Overloader{
            [&](SurfaceRenderOutput& surfaceOutput) {
                m_device.destroySurface(surfaceOutput.handle);
            },
            [&](TextureRenderOutput&) {
                log::panic("Destroying texture render output is not supported");
            }
        },
        output
    );
}

u64 Renderer::frameIndex() const {
    return m_frameNumber % m_config.renderer.maxFramesInFlight;
}

}  // namespace sl
