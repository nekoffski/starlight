#pragma once

#include "RendererProxy.hh"
#include "backend/Renderer.hh"
#include "orchestrator/RenderOrchestrator.hh"
#include "rhi/RenderDevice.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"

namespace sl {

class RenderingSystem : public NonCopyable, public NonMovable {
   public:
    explicit RenderingSystem(const Config& config);

    RendererProxy createRendererProxy() const;

   private:
    Config m_config;
    std::unique_ptr<RenderDevice> m_renderDevice;
};

}  // namespace sl
