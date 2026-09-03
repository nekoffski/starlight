#include "MetalResourcePool.hh"

namespace sl {

MetalResourcePool::MetalResourcePool(const Config&, MetalContext& ctx)
    : m_ctx(ctx) {}

Result<SurfaceHandle> MetalResourcePool::createSurface(
    std::shared_ptr<RenderSurfaceProvider> provider
) {
    auto metalProvider =
        std::dynamic_pointer_cast<MetalRenderSurfaceProvider>(provider);

    if (not metalProvider) {
        return Error::unexpected(
            ErrorCode::backendMismatch,
            "Provider is not a MetalRenderSurfaceProvider"
        );
    }

    auto layer = metalProvider->createLayer();
    SurfaceHandle handle{m_idLake.acquire<SurfaceHandle>()};

    m_surfaces.emplace(
        std::make_pair(handle, SurfaceWrapper{layer, metalProvider})
    );

    return handle;
}

void MetalResourcePool::destroySurface(SurfaceHandle handle) {
    if (auto it = m_surfaces.find(handle); it != m_surfaces.end()) {
        m_idLake.release<SurfaceHandle>(handle.id);
        m_surfaces.erase(it);
    }
}

}  // namespace sl
