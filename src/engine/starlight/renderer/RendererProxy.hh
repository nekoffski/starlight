#pragma once

#include "backend/RenderRequest.hh"
#include "executor/RenderExecutor.hh"
#include "rhi/RenderSurfaceProvider.hh"
#include "rhi/Shader.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"

namespace sl {

class RendererProxy {
    friend class RenderingSystem;

   public:
    Result<void> flushRenderer();

    Result<RenderTarget> createRenderTarget(
        std::shared_ptr<RenderSurfaceProvider> window
    );

    Result<std::future<Result<ShaderHandle>>> createShaderAsync(
        const ShaderDescription& description
    );
    Result<ShaderHandle> createShader(const ShaderDescription& description);

    void destroyShader(ShaderHandle handle);

    Result<void> submit(const RenderRequest& request);

   private:
    explicit RendererProxy(RenderExecutor::Submitter submitter);

    RenderExecutor::Submitter m_submitter;
};

}  // namespace sl
