#pragma once

#include "GraphicsPipeline.hh"
#include "RenderFrameRecorder.hh"
#include "RenderResource.hh"
#include "RenderSurfaceProvider.hh"
#include "Shader.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Functional.hh"

namespace sl {

class RenderDevice : public NonCopyable, public NonMovable {
   public:
    using RecordFrame = MoveOnlyFunction<Result<void>(RenderFrameRecorder&)>;

    virtual ~RenderDevice() = default;

    virtual Result<SurfaceHandle> attachSurface(
        std::shared_ptr<RenderSurfaceProvider> provider
    ) = 0;
    virtual void destroySurface(SurfaceHandle handle) = 0;

    virtual Result<ShaderHandle> createShader(
        const ShaderDescription& description
    ) = 0;
    virtual void destroyShader(ShaderHandle handle) = 0;

    virtual Result<GraphicsPipelineHandle> createGraphicsPipeline(
        const GraphicsPipelineDescription& description
    ) = 0;
    virtual void destroyGraphicsPipeline(GraphicsPipelineHandle handle) = 0;

    virtual Result<void> trySubmitFrame(RecordFrame callback) = 0;

    virtual void waitIdle() = 0;
};

}  // namespace sl
