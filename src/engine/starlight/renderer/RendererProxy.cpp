#include "RendererProxy.hh"

namespace sl {

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

Result<RenderOutput> RendererProxy::createRenderOutput(
    std::shared_ptr<RenderSurfaceProvider> window
) {
    RendererCreateSurfaceOutput cmd{std::move(window)};
    auto completion = cmd.completion.get_future();

    if (not m_submitter.submit(std::move(cmd))) {
        return Error::unexpected(
            ErrorCode::rendererCommandRejected,
            "Could not submit renderer create surface command"
        );
    }

    return completion.get();
}

RendererProxy::RendererProxy(RenderExecutor::Submitter submitter)
    : m_submitter(submitter) {}

}  // namespace sl
