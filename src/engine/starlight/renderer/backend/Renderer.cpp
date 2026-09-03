#include "Renderer.hh"

#include "starlight/core/Functional.hh"
#include "starlight/renderer/rhi/RenderResource.hh"

namespace sl {

Renderer::Renderer(const Config& config, RenderDevice& device)
    : m_config(config), m_device(device) {}

void Renderer::tick() {}

void Renderer::flush() {
    while (hasPendingWork()) {
        tick();
    }
}

bool Renderer::hasPendingWork() const { return false; }

Result<RenderOutput> Renderer::createOutput(
    std::shared_ptr<RenderSurfaceProvider> surfaceProvider
) {
    auto surface = m_device.createSurface(std::move(surfaceProvider));

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

}  // namespace sl
