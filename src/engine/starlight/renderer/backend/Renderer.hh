#pragma once

#include <queue>

#include "RenderRequest.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/FlatMap.hh"
#include "starlight/renderer/rhi/GraphicsPipeline.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"
#include "starlight/renderer/rhi/RenderSurfaceProvider.hh"
#include "starlight/renderer/rhi/Shader.hh"

namespace sl {

class Renderer : public NonCopyable, public NonMovable {
   public:
    explicit Renderer(const Config& config, RenderDevice& device);

    bool tick();
    void flush();

    Result<void> submit(const RenderRequest& request);

    Result<RenderTarget> createTarget(
        std::shared_ptr<RenderSurfaceProvider> surfaceProvider
    );
    void destroyTarget(RenderTarget target);

    Result<ShaderHandle> createShader(const ShaderDescription& description);
    void destroyShader(ShaderHandle handle);

   private:
    u64 frameIndex() const;

    void tryToSubmitFrame();

    Result<GraphicsPipelineHandle> resolveGraphicsPipeline(
        const GraphicsPipelineDescription& description
    );

    Result<void> recordFrame(
        RenderFrameRecorder& context, const RenderRequest& request
    );

    Config m_config;
    RenderDevice& m_device;

    std::queue<RenderRequest> m_pendingRequests;
    u64 m_frameNumber{0u};

    FlatMap<GraphicsPipelineDescription, GraphicsPipelineHandle>  // TODO: fixme
        m_graphicsPipelines;
};

}  // namespace sl
