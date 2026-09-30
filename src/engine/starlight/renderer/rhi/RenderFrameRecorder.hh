#pragma once

#include "RenderPass.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"
#include "starlight/core/Functional.hh"
#include "starlight/renderer/core/RenderResource.hh"

namespace sl {

class RenderFrameRecorder : public NonCopyable, public NonMovable {
   public:
    virtual ~RenderFrameRecorder() = default;

    virtual Result<void> renderPass(
        const RenderPassDescription& description,
        const RenderPassCallback& record
    ) = 0;
};

}  // namespace sl
