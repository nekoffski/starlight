#pragma once

#include "backend/RenderOutput.hh"
#include "executor/RenderExecutor.hh"
#include "rhi/RenderSurfaceProvider.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Error.hh"

namespace sl {

class RendererProxy {
    friend class RenderingSystem;

   public:
    Result<void> flushRenderer();

    Result<RenderOutput> createRenderOutput(
        std::shared_ptr<RenderSurfaceProvider> window
    );

   private:
    explicit RendererProxy(RenderExecutor::Submitter submitter);

    RenderExecutor::Submitter m_submitter;
};

}  // namespace sl
