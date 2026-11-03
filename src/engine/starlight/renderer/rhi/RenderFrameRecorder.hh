#pragma once

#include <kcore/Concepts.hh>
#include <kcore/Functional.hh>

#include "RenderPass.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"
#include "starlight/renderer/core/RenderResource.hh"

namespace sl {

class RenderFrameRecorder : public kc::NonCopyable, public kc::NonMovable {
   public:
    virtual ~RenderFrameRecorder() = default;

    virtual Result<void> renderPass(
        const RenderPassDescription& description,
        const RenderPassCallback& record
    ) = 0;
};

}  // namespace sl
