#include "RendererProxy.hh"

namespace sl {

Result<PrimitiveHandle> RendererProxy::createPrimitive(
    const PrimitiveUploadData& description
) {
    if (auto future = createPrimitiveAsync(description); future) {
        return future->get();
    } else {
        return Error::unexpected(future.error());
    }
}

Result<std::future<Result<PrimitiveHandle>>>
RendererProxy::createPrimitiveAsync(const PrimitiveUploadData& description) {
    RendererCreatePrimitive cmd{description};
    auto completion = cmd.completion.get_future();

    if (not m_submitter.submit(std::move(cmd))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Could not submit renderer upload primitive command"
        );
    }

    return completion;
}

Result<void> RendererProxy::destroyPrimitive(PrimitiveHandle primitive) {
    RendererDestroyPrimitive cmd{primitive};

    if (not m_submitter.submit(std::move(cmd))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Could not submit renderer destroy primitive command"
        );
    }
    return {};
}

Result<std::future<Result<ShaderHandle>>> RendererProxy::createShaderAsync(
    const ShaderDescription& description
) {
    RendererCreateShader cmd{description};
    auto completion = cmd.completion.get_future();

    if (not m_submitter.submit(std::move(cmd))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Could not submit renderer create shader command"
        );
    }

    return completion;
}

Result<ShaderHandle> RendererProxy::createShader(
    const ShaderDescription& description
) {
    if (auto future = createShaderAsync(description); future) {
        return future->get();
    } else {
        return Error::unexpected(future.error());
    }
}

Result<void> RendererProxy::destroyShader(ShaderHandle handle) {
    RendererDestroyShader cmd{handle};

    if (not m_submitter.submit(std::move(cmd))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Could not submit renderer destroy shader command"
        );
    }
    return {};
}

Result<void> RendererProxy::flushRenderer() {
    RendererFlush cmd;
    auto completion = cmd.completion.get_future();

    if (not m_submitter.submit(std::move(cmd))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Could not submit renderer flush command"
        );
    }
    completion.get();
    return {};
}

Result<RenderTarget> RendererProxy::createRenderTarget(
    std::shared_ptr<RenderSurfaceProvider> window
) {
    RendererCreateSurfaceTarget cmd{std::move(window)};
    auto completion = cmd.completion.get_future();

    if (not m_submitter.submit(std::move(cmd))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Could not submit renderer create surface command"
        );
    }

    return completion.get();
}

Result<void> RendererProxy::submit(const RenderRequest& request) {
    RendererSubmit cmd;
    cmd.request = request;
    auto completion = cmd.completion.get_future();

    if (not m_submitter.submit(std::move(cmd))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Could not submit renderer submit command"
        );
    }
    return completion.get();
}

Result<TextureHandle> RendererProxy::createTexture(
    const TextureUploadData& description
) {
    auto future = createTextureAsync(description);
    if (not future) {
        return Error::unexpected(future.error());
    }
    return future->get();
}

Result<std::future<Result<TextureHandle>>> RendererProxy::createTextureAsync(
    const TextureUploadData& description
) {
    RendererCreateTexture command{description};
    auto completion = command.completion.get_future();
    if (not m_submitter.submit(std::move(command))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Renderer create texture command rejected"
        );
    }
    return completion;
}

Result<void> RendererProxy::destroyTexture(TextureHandle handle) {
    if (not m_submitter.submit(RendererDestroyTexture{handle})) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Renderer destroy texture command rejected"
        );
    }
    return {};
}

Result<SamplerHandle> RendererProxy::createSampler(
    const SamplerDescription& description
) {
    auto future = createSamplerAsync(description);
    if (not future) {
        return Error::unexpected(future.error());
    }
    return future->get();
}

Result<std::future<Result<SamplerHandle>>> RendererProxy::createSamplerAsync(
    const SamplerDescription& description
) {
    RendererCreateSampler command{description};
    auto completion = command.completion.get_future();
    if (not m_submitter.submit(std::move(command))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Renderer create sampler command rejected"
        );
    }
    return completion;
}

Result<void> RendererProxy::destroySampler(SamplerHandle handle) {
    if (not m_submitter.submit(RendererDestroySampler{handle})) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Renderer destroy sampler command rejected"
        );
    }
    return {};
}

RendererProxy::RendererProxy(RenderExecutor::Submitter submitter)
    : m_submitter(submitter) {}

}  // namespace sl
