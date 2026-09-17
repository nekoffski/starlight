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

CA::MetalLayer* MetalResourcePool::getSurface(SurfaceHandle handle) {
    if (auto it = m_surfaces.find(handle); it != m_surfaces.end()) {
        return it->second.surface;
    }
    return nullptr;
}

void MetalResourcePool::destroySurface(SurfaceHandle handle) {
    m_surfaces.erase(handle);
}

Result<ShaderHandle> MetalResourcePool::createShader(
    const ShaderDescription& description
) {
    auto maybeShader = MetalShader::create(m_ctx, description);

    if (not maybeShader) {
        return Error::unexpected(maybeShader.error());
    }

    ShaderHandle handle{m_idLake.acquire<ShaderHandle>()};
    m_shaders.emplace(std::make_pair(handle, std::move(maybeShader.value())));
    return handle;
}

void MetalResourcePool::destroyShader(ShaderHandle handle) {
    m_shaders.erase(handle);
}

MetalShader* MetalResourcePool::getShader(ShaderHandle handle) {
    if (auto it = m_shaders.find(handle); it != m_shaders.end()) {
        return it->second.get();
    }
    return nullptr;
}

Result<GraphicsPipelineHandle> MetalResourcePool::createGraphicsPipeline(
    const GraphicsPipelineDescription& description
) {
    auto shader = getShader(description.shader);

    if (not shader) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid shader handle"
        );
    }

    auto maybePipeline =
        MetalGraphicsPipeline::create(m_ctx, *shader, description);

    if (not maybePipeline) {
        return Error::unexpected(maybePipeline.error());
    }

    GraphicsPipelineHandle handle{m_idLake.acquire<GraphicsPipelineHandle>()};
    m_graphicsPipelines.emplace(
        std::make_pair(handle, std::move(maybePipeline.value()))
    );
    return handle;
}

void MetalResourcePool::destroyGraphicsPipeline(GraphicsPipelineHandle handle) {
    m_graphicsPipelines.erase(handle);
}

MetalGraphicsPipeline* MetalResourcePool::getGraphicsPipeline(
    GraphicsPipelineHandle handle
) {
    if (auto it = m_graphicsPipelines.find(handle);
        it != m_graphicsPipelines.end()) {
        return it->second.get();
    }
    return nullptr;
}

}  // namespace sl
