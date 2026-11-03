#pragma once

#include <kcore/Concepts.hh>

#include "RendererProxy.hh"
#include "executor/RenderExecutor.hh"
#include "frontend/Renderer.hh"
#include "rhi/RenderDevice.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"

namespace sl {

class RenderingSystem : public kc::NonCopyable, public kc::NonMovable {
   public:
    explicit RenderingSystem(const Config& config);
    ~RenderingSystem();

    RendererProxy createRendererProxy();

    void stop();

   private:
    Config m_config;
    std::unique_ptr<RenderDevice> m_renderDevice;
    Renderer m_renderer;
    RenderExecutor m_renderExecutor;
};

}  // namespace sl
