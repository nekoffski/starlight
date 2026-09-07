#include "MetalResourcePool.hh"

namespace sl {

MetalResourcePool::MetalResourcePool(const Config&, MetalContext& ctx)
    : m_ctx(ctx) {}

Result<SurfaceHandle> MetalResourcePool::attachSurface(
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

    auto layer = metalProvider->getLayer();
    layer->setDevice(&m_ctx.device());

    SurfaceHandle handle{m_idLake.acquire<SurfaceHandle>()};

    m_surfaces.emplace(
        std::make_pair(handle, SurfaceWrapper{layer, metalProvider})
    );

    return handle;
}

Result<CA::MetalLayer*> MetalResourcePool::getSurface(SurfaceHandle handle) {
    if (auto it = m_surfaces.find(handle); it != m_surfaces.end()) {
        return it->second.surface;
    }
    return Error::unexpected(
        ErrorCode::invalidArgument, "Invalid surface handle"
    );
}

}  // namespace sl
