#pragma once

#include <vector>

#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/renderer/rhi/RenderFrameRecorder.hh"
#include "starlight/renderer/rhi/RenderPass.hh"

namespace sl {

class RenderGraph : public NonCopyable, public NonMovable {
    struct PassWrapper {
        RenderPassCallback record;
        std::vector<ColorAttachment> colorAttachments;
        Str label;
    };

   public:
    void addRenderPass(
        RenderPassDescription description, RenderPassCallback record
    );

    Result<void> record(RenderFrameRecorder& recorder);

   private:
    std::vector<PassWrapper> m_passes;
};

}  // namespace sl
