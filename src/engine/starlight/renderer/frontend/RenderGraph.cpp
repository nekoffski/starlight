#include "RenderGraph.hh"

namespace sl {

void RenderGraph::addRenderPass(
    RenderPassDescription description, RenderPassCallback record
) {
    m_passes.emplace_back(
        std::move(record),
        std::vector<ColorAttachment>(
            description.colorAttachments.begin(),
            description.colorAttachments.end()
        ),
        std::move(description.label)
    );
}

Result<void> RenderGraph::record(RenderFrameRecorder& recorder) {
    for (const auto& pass : m_passes) {
        RenderPassDescription description{
            .label = pass.label, .colorAttachments = pass.colorAttachments
        };

        auto res = recorder.renderPass(description, pass.record);

        if (not res) {
            if (res.error().code() == ErrorCode::renderSurfaceNotDrawable) {
                continue;
            }
            return Error::unexpected(res.error());
        }
    }
    return {};
}

}  // namespace sl
