#pragma once

#include "core/Shader.hh"
#include "executor/RendererCommandSubmitter.hh"
#include "frontend/RenderRequest.hh"
#include "rhi/RenderSurfaceProvider.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"
#include "starlight/geometry/GeometryData.hh"

namespace sl {

class RendererProxy {
   public:
    explicit RendererProxy(RendererCommandSubmitter submitter);

    Result<PrimitiveHandle> createPrimitive(const GeometryData& description);
    Result<std::future<Result<PrimitiveHandle>>> createPrimitiveAsync(
        const GeometryData& description
    );
    Result<void> destroyPrimitive(PrimitiveHandle primitive);

    Result<TextureHandle> createTexture(const TextureUploadData& description);
    Result<std::future<Result<TextureHandle>>> createTextureAsync(
        const TextureUploadData& description
    );
    Result<void> destroyTexture(TextureHandle handle);

    Result<SamplerHandle> createSampler(const SamplerDescription& description);
    Result<std::future<Result<SamplerHandle>>> createSamplerAsync(
        const SamplerDescription& description
    );
    Result<void> destroySampler(SamplerHandle handle);

    Result<void> flushRenderer();

    Result<RenderTarget> createRenderTarget(
        std::shared_ptr<RenderSurfaceProvider> window
    );

    Result<std::future<Result<ShaderHandle>>> createShaderAsync(
        const ShaderDescription& description
    );
    Result<ShaderHandle> createShader(const ShaderDescription& description);

    Result<ShaderInterface> getShaderInterface(ShaderHandle handle);

    Result<void> destroyShader(ShaderHandle handle);

    Result<void> submit(const RenderRequest& request);

   private:
    Result<void> enqueue(RendererCommand command, const char* rejectionMessage);

    template <typename Command>
    auto enqueueWithReply(Command command, const char* rejectionMessage)
        -> Result<decltype(command.completion.get_future())> {
        auto completion = command.completion.get_future();
        const auto r = enqueue(std::move(command), rejectionMessage);
        if (not r) {
            return Error::unexpected(r.error());
        }
        return completion;
    }

    RendererCommandSubmitter m_submitter;
};

}  // namespace sl
