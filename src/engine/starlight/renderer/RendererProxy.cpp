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

RendererProxy::RendererProxy(RenderExecutor::Submitter submitter)
    : m_submitter(submitter) {}

}  // namespace sl
