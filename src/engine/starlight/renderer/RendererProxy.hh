#pragma once

#include "core/Shader.hh"
#include "executor/RenderExecutor.hh"
#include "frontend/RenderRequest.hh"
#include "rhi/RenderSurfaceProvider.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"

namespace sl {

class RendererProxy {
    friend class RenderingSystem;

   public:
    Result<PrimitiveHandle> createPrimitive(
        const PrimitiveUploadData& description
    );
    Result<std::future<Result<PrimitiveHandle>>> createPrimitiveAsync(
        const PrimitiveUploadData& description
    );
    Result<void> destroyPrimitive(PrimitiveHandle primitive);

    Result<void> flushRenderer();

    Result<RenderTarget> createRenderTarget(
        std::shared_ptr<RenderSurfaceProvider> window
    );

    Result<std::future<Result<ShaderHandle>>> createShaderAsync(
        const ShaderDescription& description
    );
    Result<ShaderHandle> createShader(const ShaderDescription& description);

    Result<void> destroyShader(ShaderHandle handle);

    Result<void> submit(const RenderRequest& request);

   private:
    explicit RendererProxy(RenderExecutor::Submitter submitter);

    RenderExecutor::Submitter m_submitter;
};

}  // namespace sl
