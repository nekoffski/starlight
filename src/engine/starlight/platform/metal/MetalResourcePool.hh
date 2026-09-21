#pragma once

#include <unordered_map>

#include "Metal.hh"
#include "MetalBuffer.hh"
#include "MetalGraphicsPipeline.hh"
#include "MetalRenderSurfaceProvider.hh"
#include "MetalShader.hh"
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

    CA::MetalLayer* getSurface(SurfaceHandle handle);
    void destroySurface(SurfaceHandle handle);

    Result<ShaderHandle> createShader(const ShaderDescription& description);
    void destroyShader(ShaderHandle handle);
    MetalShader* getShader(ShaderHandle handle);

    Result<GraphicsPipelineHandle> createGraphicsPipeline(
        const GraphicsPipelineDescription& description
    );
    void destroyGraphicsPipeline(GraphicsPipelineHandle handle);
    MetalGraphicsPipeline* getGraphicsPipeline(GraphicsPipelineHandle handle);

    Result<DeviceBufferHandle> createBuffer(
        const DeviceBufferDescription& description
    );
    void destroyBuffer(DeviceBufferHandle handle);
    MetalBuffer* getBuffer(DeviceBufferHandle handle);

   private:
    IdLake m_idLake;
    MetalContext& m_ctx;

    std::unordered_map<SurfaceHandle, SurfaceWrapper> m_surfaces;
    std::unordered_map<ShaderHandle, std::unique_ptr<MetalShader>> m_shaders;
    std::unordered_map<
        GraphicsPipelineHandle, std::unique_ptr<MetalGraphicsPipeline>>
        m_graphicsPipelines;
    std::unordered_map<DeviceBufferHandle, std::unique_ptr<MetalBuffer>>
        m_buffers;
};

}  // namespace sl
