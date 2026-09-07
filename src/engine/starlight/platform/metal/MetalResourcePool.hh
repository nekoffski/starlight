#pragma once

#include <unordered_map>

#include "Metal.hh"
#include "MetalRenderSurfaceProvider.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Id.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"

namespace sl {

class MetalResourcePool : public NonCopyable, public NonMovable {
    struct SurfaceWrapper {
        CA::MetalLayer* surface;
        std::shared_ptr<MetalRenderSurfaceProvider> provider;
    };

   public:
    explicit MetalResourcePool(const Config& config, MetalContext& ctx);

    Result<SurfaceHandle> attachSurface(
        std::shared_ptr<RenderSurfaceProvider> provider
    );

    Result<CA::MetalLayer*> getSurface(SurfaceHandle handle);

   private:
    IdLake m_idLake;
    MetalContext& m_ctx;

    std::unordered_map<SurfaceHandle, SurfaceWrapper> m_surfaces;
};

}  // namespace sl
