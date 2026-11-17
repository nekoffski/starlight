#pragma once

#include <kcore/Concepts.hh>
#include <kcore/containers/FlatMap.hh>
#include <queue>
#include <span>

#include "RenderRequest.hh"
#include "RendererResourcePool.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/core/Primitive.hh"
#include "starlight/renderer/core/Shader.hh"
#include "starlight/renderer/rhi/GraphicsPipeline.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"
#include "starlight/renderer/rhi/RenderSurfaceProvider.hh"

namespace sl {

class Renderer : public kc::NonCopyable, public kc::NonMovable {
   public:
    explicit Renderer(const Config& config, RenderDevice& device);

    bool tick();
    void flush();

    Result<void> submit(const RenderRequest& request);

    Result<RenderTarget> createTarget(
        std::shared_ptr<RenderSurfaceProvider> surfaceProvider
    );
    void destroyTarget(RenderTarget target);

    Result<TextureHandle> createTexture(const TextureUploadData& data) {
        return m_device.createTexture(data);
    }
    Result<SamplerHandle> createSampler(const SamplerDescription& description) {
        return m_device.createSampler(description);
    }
    void destroyTexture(TextureHandle handle);
    void destroySampler(SamplerHandle handle);

    Result<ShaderHandle> createShader(const ShaderDescription& description) {
        return m_device.createShader(description);
    }

    Result<ShaderInterface> getShaderInterface(ShaderHandle handle) const;

    void destroyShader(ShaderHandle handle);

    Result<PrimitiveHandle> createPrimitive(const GeometryData& description) {
        return m_resourcePool.createPrimitive(description);
    }

    void destroyPrimitive(PrimitiveHandle handle);

   private:
    using GraphicsPipelineCache =
        kc::FlatMap<GraphicsPipelineDescription, GraphicsPipelineHandle>;

    Result<void> validateRequest(const RenderRequest& request);

    void tryToSubmitFrame();

    Result<GraphicsPipelineHandle> resolveGraphicsPipeline(
        const GraphicsPipelineDescription& description
    );

    Result<void> recordFrame(
        RenderFrameRecorder& context, const RenderRequest& request
    );

    Result<void> recordView(
        RenderPassEncoder& encoder, const RenderRequest& request,
        std::span<const ShaderParameterBlock> viewParameters
    );

    Result<void> recordItem(
        RenderPassEncoder& encoder, const RenderItem& item,
        std::span<const ShaderParameterBlock> viewParameters,
        const ShaderResourceDependencies& resources
    );

    Result<void> recordPrimitive(
        RenderPassEncoder& encoder, ShaderHandle shader,
        const PrimitiveDraw& draw
    );

    Config m_config;
    RenderDevice& m_device;

    std::queue<RenderRequest> m_pendingRequests;

    GraphicsPipelineCache m_graphicsPipelines;

    RendererResourcePool m_resourcePool;
};

}  // namespace sl
