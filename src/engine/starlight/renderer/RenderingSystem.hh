#pragma once

#include "RendererProxy.hh"
#include "backend/Renderer.hh"
#include "executor/RenderExecutor.hh"
#include "rhi/RenderDevice.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Core.hh"

namespace sl {

class RenderingSystem : public NonCopyable, public NonMovable {
   public:
    explicit RenderingSystem(const Config& config);
    ~RenderingSystem();

    RendererProxy createRendererProxy() const;

    void stop();

   private:
    Config m_config;
    std::unique_ptr<RenderDevice> m_renderDevice;
    Renderer m_renderer;
    RenderExecutor m_renderExecutor;
};

}  // namespace sl
