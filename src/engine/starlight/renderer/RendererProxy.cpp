#include "RendererProxy.hh"

namespace sl {
namespace {

template <typename T>
Result<T> waitForReply(Result<std::future<Result<T>>> completion) {
    if (not completion) {
        return Error::unexpected(completion.error());
    }
    return completion->get();
}

Result<void> waitForReply(Result<std::future<void>> completion) {
    if (not completion) {
        return Error::unexpected(completion.error());
    }
    completion->get();
    return {};
}

}  // namespace

RendererProxy::RendererProxy(RenderExecutor::Submitter submitter)
    : m_submitter(std::move(submitter)) {}

Result<PrimitiveHandle> RendererProxy::createPrimitive(
    const GeometryData& description
) {
    return waitForReply(createPrimitiveAsync(description));
}

Result<std::future<Result<PrimitiveHandle>>>
RendererProxy::createPrimitiveAsync(const GeometryData& description) {
    return enqueueWithReply(
        RendererCreatePrimitive{description},
        "Could not submit renderer upload primitive command"
    );
}

Result<void> RendererProxy::destroyPrimitive(PrimitiveHandle primitive) {
    return enqueue(
        RendererDestroyPrimitive{primitive},
        "Could not submit renderer destroy primitive command"
    );
}

Result<ShaderHandle> RendererProxy::createShader(
    const ShaderDescription& description
) {
    return waitForReply(createShaderAsync(description));
}

Result<std::future<Result<ShaderHandle>>> RendererProxy::createShaderAsync(
    const ShaderDescription& description
) {
    return enqueueWithReply(
        RendererCreateShader{description},
        "Could not submit renderer create shader command"
    );
}

Result<void> RendererProxy::destroyShader(ShaderHandle handle) {
    return enqueue(
        RendererDestroyShader{handle},
        "Could not submit renderer destroy shader command"
    );
}

Result<ShaderInterface> RendererProxy::getShaderInterface(ShaderHandle handle) {
    return waitForReply(enqueueWithReply(
        RendererGetShaderInterface{handle},
        "Renderer shader interface command rejected"
    ));
}

Result<void> RendererProxy::flushRenderer() {
    return waitForReply(enqueueWithReply(
        RendererFlush{}, "Could not submit renderer flush command"
    ));
}

Result<RenderTarget> RendererProxy::createRenderTarget(
    std::shared_ptr<RenderSurfaceProvider> window
) {
    return waitForReply(enqueueWithReply(
        RendererCreateSurfaceTarget{std::move(window)},
        "Could not submit renderer create surface command"
    ));
}

Result<void> RendererProxy::submit(const RenderRequest& request) {
    return waitForReply(enqueueWithReply(
        RendererSubmit{.request = request},
        "Could not submit renderer submit command"
    ));
}

Result<TextureHandle> RendererProxy::createTexture(
    const TextureUploadData& description
) {
    return waitForReply(createTextureAsync(description));
}

Result<std::future<Result<TextureHandle>>> RendererProxy::createTextureAsync(
    const TextureUploadData& description
) {
    return enqueueWithReply(
        RendererCreateTexture{description},
        "Renderer create texture command rejected"
    );
}

Result<void> RendererProxy::destroyTexture(TextureHandle handle) {
    return enqueue(
        RendererDestroyTexture{handle},
        "Renderer destroy texture command rejected"
    );
}

Result<SamplerHandle> RendererProxy::createSampler(
    const SamplerDescription& description
) {
    return waitForReply(createSamplerAsync(description));
}

Result<std::future<Result<SamplerHandle>>> RendererProxy::createSamplerAsync(
    const SamplerDescription& description
) {
    return enqueueWithReply(
        RendererCreateSampler{description},
        "Renderer create sampler command rejected"
    );
}

Result<void> RendererProxy::destroySampler(SamplerHandle handle) {
    return enqueue(
        RendererDestroySampler{handle},
        "Renderer destroy sampler command rejected"
    );
}

Result<void> RendererProxy::enqueue(
    RendererCommand command, const char* rejectionMessage
) {
    if (not m_submitter.submit(std::move(command))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected, rejectionMessage
        );
    }
    return {};
}

}  // namespace sl
